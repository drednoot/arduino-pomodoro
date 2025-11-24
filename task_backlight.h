#ifndef ARDUINO_POMODORO_TASK_BACKLIGHT_H_
#define ARDUINO_POMODORO_TASK_BACKLIGHT_H_

#include "lcd_timer.h"
#include "ns_task.h"
#include "timer.h"

template<uint32_t noBacklightTimeout>
class Backlight : public Task {
  public:
    Backlight(LcdTimer* lcdTimer)
      : m_lcdTimer(lcdTimer)
    {
    }

    void setup() override
    {
      m_timer.start();
      m_done = false;
      m_lcdTimer->setBacklightEnabled(true);
    }

    void sync() override
    {
      if (m_timer.timePassed() >= noBacklightTimeout) {
        m_done = true;
        m_lcdTimer->setBacklightEnabled(false);
      }
    }

    boolean isDone() const override
    {
      return m_done;
    }

    boolean syncWhenDone() const override
    {
      return false;
    }

  private:
    LcdTimer* m_lcdTimer;
    Timer m_timer;
    boolean m_done;
};

#endif // ARDUINO_POMODORO_TASK_BACKLIGHT_H_
