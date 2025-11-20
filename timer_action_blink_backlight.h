#ifndef ARDUINO_POMODORO_TIMER_ACTION_BLINK_BACKLIGHT_H_
#define ARDUINO_POMODORO_TIMER_ACTION_BLINK_BACKLIGHT_H_

#include "ns_timer_action.h"
#include "lcd_timer.h"

class BlinkBacklight;

using BlinkBacklightAction = TimerAction<void(BlinkBacklight::*)(), BlinkBacklight*>;

class BlinkBacklight : public BlinkBacklightAction {
  public:
    BlinkBacklight(LcdTimer* lcdTimer, uint32_t time)
      : BlinkBacklightAction(time, this, &BlinkBacklight::blink)
      , m_lcdTimer(lcdTimer)
      , m_state(true)
    {
    }

    void reset()
    {
      m_state = true;
      m_lcdTimer->setBacklightEnabled(true);
      BlinkBacklightAction::reset();
    }

  private:
    void blink()
    {
      m_state = !m_state;
      m_lcdTimer->setBacklightEnabled(m_state);
    }

    boolean m_state;
    LcdTimer* m_lcdTimer;
};

#endif // ARDUINO_POMODORO_TIMER_ACTION_BLINK_BACKLIGHT_H_
