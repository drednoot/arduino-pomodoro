#ifndef ARDUINO_POMODORO_TASKS_BUZZER_H_
#define ARDUINO_POMODORO_TASKS_BUZZER_H_

#include "ns_task.h"
#include "timer.h"

template<uint8_t pin, uint32_t period, uint32_t buzzTime, uint16_t freq>
class Buzzer : public Task {
  public:
    Buzzer()
      : m_done(false)
    {
    }

    void setup() override
    {
      pinMode(pin, OUTPUT);
      m_timer.start();
      m_done = false;
    }

    void sync() override
    {
      uint32_t passed = m_timer.timePassed();
      if (passed >= buzzTime) {
        m_done = true;
        noTone(pin);
        return;
      }

      if ((passed / period) % 2 == 0) {
        tone(pin, freq);
      } else {
        noTone(pin);
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
    Timer m_timer;
    boolean m_done;
};

#endif // ARDUINO_POMODORO_TASKS_BUZZER_H_
