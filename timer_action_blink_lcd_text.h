#ifndef ARDUINO_POMODORO_TIMER_ACTION_BLINK_LCD_TEXT_H_
#define ARDUINO_POMODORO_TIMER_ACTION_BLINK_LCD_TEXT_H_

#include "ns_timer_action.h"
#include "lcd_timer.h"

class BlinkLcdText;

using BlinkLcdTextAction = TimerAction<void(BlinkLcdText::*)(), BlinkLcdText*>;

class BlinkLcdText : public BlinkLcdTextAction {
  public:
    BlinkLcdText(LcdTimer* lcdTimer, uint32_t time)
      : BlinkLcdTextAction(time, this, &BlinkLcdText::blink)
      , m_lcdTimer(lcdTimer)
      , m_state(true)
    {
    }

    void reset()
    {
      m_state = true;
      m_lcdTimer->setTextVisible(true);
      BlinkLcdTextAction::reset();
    }

  private:
    void blink()
    {
      m_state = !m_state;
      m_lcdTimer->setTextVisible(m_state);
    }

    LcdTimer* m_lcdTimer;
    boolean m_state;
};

#endif // ARDUINO_POMODORO_TIMER_ACTION_BLINK_LCD_TEXT_H_
