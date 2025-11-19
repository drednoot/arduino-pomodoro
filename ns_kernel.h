#ifndef ARDUINO_POMODORO_NS_KERNEL_H_
#define ARDUINO_POMODORO_NS_KERNEL_H_

#include "array.h"
#include "time.h"
#include "ns_task.h"
#include "task_timer_countdown.h"
#include "task_timer_blink_dots.h"

LcdTimer lcd_timer(0x27, 16, 2);

TimerCountdown<25, 0> workTimerCountdown(&lcd_timer);
BlinkDots blinkDots(&lcd_timer);

enum State {
  STATE_AWAIT_BEGIN = 0,
  STATE_TIMER_COUNTDOWN,
  STATE_PAUSE,
  STATE_AWAIT_NEXT_CYCLE,
};

template<uint8_t max_tasks>
class Kernel {
  public:
    Kernel()
      // TODO: set STATE_AWAIT_BEGIN
      : m_state { STATE_TIMER_COUNTDOWN }
    {
    }

    void setup()
    {
      lcd_timer.setup();

      setState(m_state);
    }

    void sync()
    {
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

    Array<Task*, max_tasks> m_tasks;
    State m_state;
};

#endif // ARDUINO_POMODORO_NS_KERNEL_H_
