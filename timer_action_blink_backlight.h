#ifndef ARDUINO_POMODORO_TIMER_ACTION_BLINK_BACKLIGHT_H_
#define ARDUINO_POMODORO_TIMER_ACTION_BLINK_BACKLIGHT_H_

#include "ns_timer_action.h"
#include "lcd_timer.h"

template<uint32_t period>
class BlinkBacklightAction;

template<uint32_t period>
using BlinkBacklightTimerAction = TimerAction<void(BlinkBacklightAction<period>::*)(), BlinkBacklightAction<period>*>;

template<uint32_t period>
class BlinkBacklightAction : public BlinkBacklightTimerAction<period> {
  public:
    BlinkBacklightAction(LcdTimer* lcdTimer)
      : BlinkBacklightTimerAction<period>(period, this, &BlinkBacklightAction::blink)
      , m_lcdTimer(lcdTimer)
      , m_state(true)
    {
    }

    void reset()
    {
      m_state = true;
      m_lcdTimer->setBacklightEnabled(true);
      BlinkBacklightTimerAction<period>::reset();
    }

  private:
    void blink()
    {
      m_state = !m_state;
      m_lcdTimer->setBacklightEnabled(m_state);
    }

    LcdTimer* m_lcdTimer;
    boolean m_state;
};

#endif // ARDUINO_POMODORO_TIMER_ACTION_BLINK_BACKLIGHT_H_
