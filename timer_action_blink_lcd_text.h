#ifndef ARDUINO_POMODORO_TIMER_ACTION_BLINK_LCD_TEXT_H_
#define ARDUINO_POMODORO_TIMER_ACTION_BLINK_LCD_TEXT_H_

#include "ns_timer_action.h"
#include "lcd_timer.h"

template<uint32_t period>
class BlinkLcdTextAction;

template<uint32_t period>
using BlinkLcdTextTimerAction = TimerAction<void(BlinkLcdTextAction<period>::*)(), BlinkLcdTextAction<period>*>;

template<uint32_t period>
class BlinkLcdTextAction : public BlinkLcdTextTimerAction<period> {
  public:
    BlinkLcdTextAction(LcdTimer* lcdTimer)
      : BlinkLcdTextTimerAction<period>(period, this, &BlinkLcdTextAction::blink)
      , m_lcdTimer(lcdTimer)
      , m_state(true)
    {
    }

    void reset()
    {
      m_state = true;
      m_lcdTimer->setTextVisible(true);
      BlinkLcdTextAction<period>::reset();
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
