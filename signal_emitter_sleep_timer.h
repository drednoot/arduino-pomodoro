#ifndef ARDUINO_POMODORO_SIGNAL_EMITTER_SLEEP_TIMER_H_
#define ARDUINO_POMODORO_SIGNAL_EMITTER_SLEEP_TIMER_H_

#include "ns_signal_emitter.h"
#include "ns_signals.h"
#include "timer.h"

template<uint32_t timeUntilSleep>
class SleepTimer : public SignalEmitter {
  public:
    SleepTimer()
    {
    }

    void sync() override
    {
      if (m_timer.timePassed() >= timeUntilSleep) {
        m_emittingSignals = SIG_SHUTDOWN | SIG_ACTIVATE;
      }
    }

    Signals signals() override
    {
      return m_emittingSignals;
    }

    void setSignalsHandled() override
    {
      m_emittingSignals = SIG_NO_SIGNAL;
    }

    void reset()
    {
      m_timer.start();
    }

    void stop()
    {
      m_timer.end();
    }

  private:
    Timer m_timer;
    Signals m_emittingSignals;
};

#endif // ARDUINO_POMODORO_SIGNAL_EMITTER_SLEEP_TIMER_H_
