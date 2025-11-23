#ifndef ARDUINO_POMODORO_TASK_TIMER_BLINK_DOTS_H_
#define ARDUINO_POMODORO_TASK_TIMER_BLINK_DOTS_H_

#include "ns_task.h"
#include "lcd_timer.h"
#include "ns_timer_action.h"

template<uint32_t period>
class BlinkDots : public Task {
  public:
    using BlinkDotsAction = TimerAction<void(BlinkDots::*)(), BlinkDots*>;

    BlinkDots(LcdTimer* lcdTimer)
      : m_lcdTimer(lcdTimer)
      , m_blinkDotsAction(period, this, &BlinkDots::blinkDots)
      , m_isDotsVisible(true)
    {
    }

    void setup() override
    {
      m_isDotsVisible = true;
      m_lcdTimer->setDotsVisible(true);
    }

    void sync() override
    {
      m_blinkDotsAction.sync();
    }

    boolean isDone() const override 
    {
      return true;
    }

    boolean syncWhenDone() const override
    {
      return true;
    }

  private:
    void blinkDots()
    {
      m_isDotsVisible = !m_isDotsVisible;
      m_lcdTimer->setDotsVisible(m_isDotsVisible);
    }

    LcdTimer* m_lcdTimer;
    BlinkDotsAction m_blinkDotsAction;
    boolean m_isDotsVisible;
};

#endif // ARDUINO_POMODORO_TASK_TIMER_BLINK_DOTS_H_
