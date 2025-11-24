#ifndef ARDUINO_POMODORO_TASK_BLINK_TIMER_H_
#define ARDUINO_POMODORO_TASK_BLINK_TIMER_H_

#include "lcd_timer.h"
#include "ns_task.h"
#include "timer_action_blink_timer.h"

template<uint32_t period>
class BlinkTimer : public Task {
  public:
    BlinkTimer(LcdTimer* lcdTimer)
      : m_blinkTimerAction(lcdTimer)
    {
    }
    
    void setup() override
    {
      m_blinkTimerAction.reset();
    }

    void sync() override
    {
      m_blinkTimerAction.sync();
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
    BlinkTimerAction<period> m_blinkTimerAction;
};

#endif // ARDUINO_POMODORO_TASK_BLINK_TIMER_H_
