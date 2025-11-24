#ifndef ARDUINO_POMODORO_TASKS_TIMER_COUNTDOWN_H_
#define ARDUINO_POMODORO_TASKS_TIMER_COUNTDOWN_H_

#include "ns_task.h"
#include "lcd_timer.h"
#include "time.h"

template<uint8_t minutes, uint8_t seconds>
class TimerCountdown : public Task {
  public:
    using TimerCountdownAction = TimerAction<void(TimerCountdown::*)(), TimerCountdown*>;

    TimerCountdown(LcdTimer* lcdTimer)
      : m_lcdTimer(lcdTimer)
      , m_timerAction(1000, this, &TimerCountdown::decrementTime)
      , m_time(Time {minutes, seconds})
    {
    }

    void setup() override
    {
      m_time = Time {minutes, seconds};
      m_lcdTimer->setTime(m_time);
    }

    void sync() override
    {
      m_timerAction.sync();
    }

    boolean isDone() const override
    {
      return m_time.isZero();
    }

    boolean syncWhenDone() const override
    {
      return false;
    }

    void clear() override
    {
    }

    Time startingTime() const
    {
      return Time {minutes, seconds};
    }

  private:
    void decrementTime()
    {
      --m_time;
      m_lcdTimer->setTime(m_time);
    }

    LcdTimer* m_lcdTimer;
    TimerCountdownAction m_timerAction;
    Time m_time;
};

#endif // ARDUINO_POMODORO_TASKS_TIMER_COUNTDOWN_H_
