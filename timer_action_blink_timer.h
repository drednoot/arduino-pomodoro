#ifndef ARDUINO_POMODORO_TIMER_ACTION_BLINK_TIMER_H_
#define ARDUINO_POMODORO_TIMER_ACTION_BLINK_TIMER_H_

#include "ns_timer_action.h"
#include "lcd_timer.h"

template<uint32_t period>
class BlinkTimerAction;

template<uint32_t period>
using BlinkTimerTimerAction = TimerAction<void(BlinkTimerAction<period>::*)(), BlinkTimerAction<period>*>;

template<uint32_t period>
class BlinkTimerAction : public BlinkTimerTimerAction<period> {
  public:
    BlinkTimerAction(LcdTimer* lcdTimer)
      : BlinkTimerTimerAction<period>(period, this, &BlinkTimerAction<period>::blink)
      , m_lcdTimer(lcdTimer)
      , m_state(true)
    {
    }

    void reset()
    {
      m_state = true;
      m_lcdTimer->setTimerVisible(true);
      BlinkTimerTimerAction<period>::reset();
    }

  private:
    void blink()
    {
      m_state = !m_state;
      m_lcdTimer->setTimerVisible(m_state);
    }

    LcdTimer* m_lcdTimer;
    boolean m_state;
};

#endif // ARDUINO_POMODORO_TIMER_ACTION_BLINK_TIMER_H_
