#ifndef ARDUINO_POMODORO_TIMER_ACTION_BLINK_TIMER_H_
#define ARDUINO_POMODORO_TIMER_ACTION_BLINK_TIMER_H_

#include "ns_timer_action.h"
#include "lcd_timer.h"

class BlinkTimer;

using BlinkTimerTimerAction = TimerAction<void(BlinkTimer::*)(), BlinkTimer*>;

class BlinkTimer : public BlinkTimerTimerAction {
  public:
    BlinkTimer(LcdTimer* lcdTimer, uint32_t time)
      : BlinkTimerTimerAction(time, this, &BlinkTimer::blink)
      , m_lcdTimer(lcdTimer)
      , m_state(true)
    {
    }

    void reset()
    {
      m_state = true;
      m_lcdTimer->setTimerVisible(true);
      BlinkTimerTimerAction::reset();
    }

  private:
    void blink()
    {
      m_state = !m_state;
      m_lcdTimer->setTimerVisible(m_state);
    }

    boolean m_state;
    LcdTimer* m_lcdTimer;
};

#endif // ARDUINO_POMODORO_TIMER_ACTION_BLINK_TIMER_H_
