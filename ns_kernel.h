#ifndef ARDUINO_POMODORO_NS_KERNEL_H_
#define ARDUINO_POMODORO_NS_KERNEL_H_

#include "array.h"
#include "time.h"
#include "ns_task.h"
#include "ns_signals.h"
#include "task_timer_countdown.h"
#include "task_timer_blink_dots.h"
#include "signal_emitter_push_button.h"
#include "timer_action_blink_timer.h"
#include "timer_action_blink_lcd_text.h"
#include "timer_action_blink_backlight.h"
#include "once.h"

#define ARDUINO_RESET_PIN 10

LcdTimer lcdTimer(0x27, 16, 2);

TimerCountdown<25, 0> workTimerCountdown(&lcdTimer);
BlinkDots blinkDots(&lcdTimer);

PushButton<11> pushButton;

BlinkTimer blinkTimer(&lcdTimer, 300);
BlinkLcdText blinkText(&lcdTimer, 300);
BlinkBacklight blinkBacklight(&lcdTimer, 300);

const static uint8_t maxTasks = 2;
const static uint8_t maxSignalEmitters = 1;

enum State {
  STATE_AWAIT_BEGIN = 0,
  STATE_TIMER_COUNTDOWN,
  STATE_PAUSE,
  STATE_AWAIT_NEXT_CYCLE,
};

class Kernel {
  public:
    Kernel()
      // TODO: set STATE_AWAIT_BEGIN
      : m_state { STATE_TIMER_COUNTDOWN }
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

      uint8_t doneCount = 0;
      for (uint8_t i = 0, size = m_tasks.size(); i < size; ++i) {
        if (m_tasks[i]->isDone()) {
          ++doneCount;
          if (!m_tasks[i]->syncWhenDone()) continue;
        }
        m_tasks[i]->sync();
      }
    }

  private:
    void setupTasks()
    {
      for (uint8_t i = 0, size = m_tasks.size(); i < size; ++i) {
        m_tasks[i]->setup();
      }
    }

    void setState(State state)
    {
      m_tasks.clear();
      switch(state) {
      case STATE_AWAIT_BEGIN:
        break;
      case STATE_TIMER_COUNTDOWN:
        m_tasks.push(&workTimerCountdown);
        m_tasks.push(&blinkDots);
        break;
      case STATE_PAUSE:
        break;
      case STATE_AWAIT_NEXT_CYCLE:
        break;
      }
      m_state = state;
      setupTasks();
    }

    void handleSignals(SignalEmitter* signalEmitter)
    {
      Signal toHandle = signalEmitter->signals();
      boolean buttonPushed = toHandle & SIG_BUTTON_PUSHED;
      toHandle &= ~SIG_BUTTON_PUSHED;

      resetEffectTimers(toHandle);

      switch (toHandle) {
      case SIG_PAUSE:
        if (!buttonPushed) break;
        break;
      case SIG_TIMER_RESET:
        blinkTimer.sync();
        break;
      case SIG_HARD_RESET:
        blinkText.sync();
        break;
      case SIG_SHUTDOWN:
        blinkBacklight.sync();
        break;
      case SIG_FULL_RESET:
        digitalWrite(ARDUINO_RESET_PIN, LOW);
        break;
      case SIG_BUTTON_PUSHED:
      case SIG_NO_SIGNAL:
        break;
      }

      signalEmitter->setSignalsHandled();
    }

    void resetEffectTimers(Signal signal)
    {
        if (m_signalOnce.set(signal)) {
          blinkTimer.reset();
          blinkText.reset();
          blinkBacklight.reset();
        }
    }

    Array<Task*, maxTasks> m_tasks;
    Array<SignalEmitter*, maxSignalEmitters> m_signalEmitters;
    State m_state;
    Once<Signal> m_signalOnce;
};

#endif // ARDUINO_POMODORO_NS_KERNEL_H_
