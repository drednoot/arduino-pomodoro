#ifndef ARDUINO_POMODORO_TIMER_H_
#define ARDUINO_POMODORO_TIMER_H_

class Timer {
  public:
    Timer()
      : m_startTime(0)
    {
    }

    void start()
    {
      m_startTime = millis();
    }

    uint32_t end()
    {
      uint32_t delta = millis() - m_startTime;
      m_startTime = 0;

      return delta;
    }

    uint32_t timePassed()
    {
      if (m_startTime == 0) return 0;
      return millis() - m_startTime;
    }

  private:
    uint32_t m_startTime;
};

#endif // ARDUINO_POMODORO_TIMER_H_
