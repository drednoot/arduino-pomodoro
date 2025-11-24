#ifndef ARDUINO_POMODORO_NS_KERNEL_H_
#define ARDUINO_POMODORO_NS_KERNEL_H_

#include "array.h"
#include "time.h"
#include "ns_task.h"
#include "ns_signals.h"
#include "task_timer_countdown.h"
#include "task_timer_blink_dots.h"
#include "task_buzzer.h"
#include "task_backlight.h"
#include "task_blink_timer.h"
#include "signal_emitter_push_button.h"
#include "timer_action_blink_timer.h"
#include "timer_action_blink_lcd_text.h"
#include "timer_action_blink_backlight.h"
#include "once.h"

#define ARDUINO_RESET_PIN 10

LcdTimer lcdTimer(0x27, 16, 2, Time {0, 10});

TimerCountdown<0, 10> workTimerCountdown(&lcdTimer);
TimerCountdown<0, 10> restTimerCountdown(&lcdTimer);
TimerCountdown<0, 15> longRestTimerCountdown(&lcdTimer);
BlinkDots<800> blinkDots(&lcdTimer);
Buzzer<12, 150, 5000, 440> buzzer;
Backlight<10000> backlight(&lcdTimer);
BlinkTimer<500> blinkTimer(&lcdTimer);

PushButton<11> pushButton;

BlinkTimerAction<300> blinkTimerAction(&lcdTimer);
BlinkLcdTextAction<300> blinkTextAction(&lcdTimer);
BlinkBacklightAction<300> blinkBacklightAction(&lcdTimer);

const static uint8_t maxTasks = 4;
const static uint8_t maxSignalEmitters = 1;

enum State {
  STATE_AWAIT_FIRST_CYCLE = 0,
  STATE_AWAIT_NEXT_CYCLE,
  STATE_WORK_TIMER_COUNTDOWN,
  STATE_REST_TIMER_COUNTDOWN,
  STATE_WORK_PAUSE,
  STATE_REST_PAUSE,
  // STATE_SLEEP, possibly?
};

class Kernel {
  public:
    Kernel()
      : m_state { STATE_AWAIT_FIRST_CYCLE }
      , m_stopTasks { false }
    {
    }

    void setup()
    {
      pinMode(ARDUINO_RESET_PIN, OUTPUT);
      digitalWrite(ARDUINO_RESET_PIN, HIGH);

      lcdTimer.setup();
      pushButton.setup();
      m_signalEmitters.push(&pushButton);

      setState(m_state);
    }

    void sync()
    {
      for (uint8_t i = 0, size = m_signalEmitters.size(); i < size; ++i) {
        m_signalEmitters[i]->sync();
        handleSignals(m_signalEmitters[i]);
      }

      if (m_stopTasks) return;

      uint8_t doneCount = 0;
      for (uint8_t i = 0, size = m_tasks.size(); i < size; ++i) {
        if (m_tasks[i]->isDone()) {
          ++doneCount;
          if (!m_tasks[i]->syncWhenDone()) continue;
        }
        m_tasks[i]->sync();
      }

      if (doneCount == m_tasks.size()) {
        proposeAllTasksFinished();
      }
    }

  private:
    void setState(State state)
    {
      clearTasks();
      switch(state) {
      case STATE_AWAIT_FIRST_CYCLE:
        lcdTimer.setBacklightEnabled(true);
        lcdTimer.setIsWork(true);
        lcdTimer.setTime(workTimerCountdown.startingTime());

        blinkDots.setup();
        m_tasks.push(&blinkDots);
        break;
      case STATE_WORK_TIMER_COUNTDOWN:
        if (m_state != STATE_WORK_PAUSE) workTimerCountdown.setup();
        m_tasks.push(&workTimerCountdown);

        lcdTimer.setIsWork(true);

        blinkDots.setup();
        m_tasks.push(&blinkDots);

        backlight.setup();
        m_tasks.push(&backlight);
        break;
      case STATE_WORK_PAUSE:
        blinkTimer.setup();
        m_tasks.push(&blinkTimer);
        break;
      case STATE_REST_TIMER_COUNTDOWN:
        if (m_state != STATE_REST_PAUSE) {
          if (lcdTimer.pomodoro() == POMO_POMODORO_MAX_COUNT) {
            longRestTimerCountdown.setup();
          } else {
            restTimerCountdown.setup();
          }
          
          buzzer.setup();
        }
        if (lcdTimer.pomodoro() == POMO_POMODORO_MAX_COUNT) {
          m_tasks.push(&longRestTimerCountdown);
        } else {
          m_tasks.push(&restTimerCountdown);
        }

        m_tasks.push(&buzzer);

        lcdTimer.setIsWork(false);

        blinkDots.setup();
        m_tasks.push(&blinkDots);

        backlight.setup();
        m_tasks.push(&backlight);

        break;
      case STATE_REST_PAUSE:
        blinkTimer.setup();
        m_tasks.push(&blinkTimer);
        break;
      case STATE_AWAIT_NEXT_CYCLE:
        lcdTimer.setIsWork(true);
        lcdTimer.setTime(workTimerCountdown.startingTime());

        lcdTimer.setCycles(lcdTimer.cycles() + 1);
        lcdTimer.setPomodoro(lcdTimer.pomodoro() % 4 + 1);

        blinkDots.setup();
        m_tasks.push(&blinkDots);

        buzzer.setup();
        m_tasks.push(&buzzer);
        break;
      }
      m_state = state;
    }

    void clearTasks()
    {
      for (int i = 0; i < m_tasks.size(); ++i) {
        m_tasks[i]->clear();
      }
      m_tasks.clear();
    }

    void proposeAllTasksFinished()
    {
      switch(m_state) {
      case STATE_AWAIT_FIRST_CYCLE:
      case STATE_AWAIT_NEXT_CYCLE:
      case STATE_WORK_PAUSE:
      case STATE_REST_PAUSE:
        break;
      case STATE_WORK_TIMER_COUNTDOWN:
        setState(STATE_REST_TIMER_COUNTDOWN);
        break;
      case STATE_REST_TIMER_COUNTDOWN:
        setState(STATE_AWAIT_NEXT_CYCLE);
        break;
      }
    }

    void handleSignals(SignalEmitter* signalEmitter)
    {
      Signals toHandle = signalEmitter->signals();
      if (toHandle == SIG_NO_SIGNAL) return;

      boolean buttonPushed = toHandle & SIG_BUTTON_PUSHED;
      toHandle &= ~SIG_BUTTON_PUSHED;

      resetEffectTimers(toHandle);

      switch (static_cast<Signal>(toHandle)) {
      case SIG_PAUSE:
        if (buttonPushed) handlePauseSignal();
        break;
      case SIG_TIMER_RESET:
        if (buttonPushed) {
          switch(m_state) {
          case STATE_AWAIT_FIRST_CYCLE:
          case STATE_AWAIT_NEXT_CYCLE:
            break;
          case STATE_WORK_TIMER_COUNTDOWN:
          case STATE_WORK_PAUSE:
            workTimerCountdown.setup();
            break;
          case STATE_REST_TIMER_COUNTDOWN:
          case STATE_REST_PAUSE:
            restTimerCountdown.setup();
            longRestTimerCountdown.setup();
            break;
          }
        } else {
          blinkTimerAction.sync();
        }
        break;
      case SIG_HARD_RESET:
        if (!buttonPushed) blinkTextAction.sync();
        break;
      case SIG_SHUTDOWN:
        if (!buttonPushed) blinkBacklightAction.sync();
        break;
      case SIG_FULL_RESET:
        digitalWrite(ARDUINO_RESET_PIN, LOW);
        break;
      case SIG_BUTTON_PUSHED:
      case SIG_NO_SIGNAL:
        break;
      }

      if (buttonPushed) {
        m_stopTasks = false;
        resetEffectTimers(toHandle, true);
      } else {
        m_stopTasks = true;
      }

      signalEmitter->setSignalsHandled();
    }

    void resetEffectTimers(Signals signals, boolean forced = false)
    {
      if (forced || m_signalsOnce.set(signals)) {
        blinkTimerAction.reset();
        blinkTextAction.reset();
        blinkBacklightAction.reset();
      }
    }

    void handlePauseSignal()
    {
      switch(m_state) {
      case STATE_AWAIT_FIRST_CYCLE:
        setState(STATE_WORK_TIMER_COUNTDOWN);
        break;
      case STATE_WORK_TIMER_COUNTDOWN:
        setState(STATE_WORK_PAUSE);
        break;
      case STATE_REST_TIMER_COUNTDOWN:
        setState(STATE_REST_PAUSE);
        break;
      case STATE_WORK_PAUSE:
        setState(STATE_WORK_TIMER_COUNTDOWN);
        break;
      case STATE_REST_PAUSE:
        setState(STATE_REST_TIMER_COUNTDOWN);
        break;
      case STATE_AWAIT_NEXT_CYCLE:
        setState(STATE_WORK_TIMER_COUNTDOWN);
      }
    }

    Array<Task*, maxTasks> m_tasks;
    Array<SignalEmitter*, maxSignalEmitters> m_signalEmitters;
    State m_state;
    Once<Signals> m_signalsOnce;
    boolean m_stopTasks;
};

#endif // ARDUINO_POMODORO_NS_KERNEL_H_
