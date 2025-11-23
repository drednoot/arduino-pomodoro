#ifndef ARDUINO_POMODORO_SIGNAL_EMITTER_PUSH_BUTTON_H_
#define ARDUINO_POMODORO_SIGNAL_EMITTER_PUSH_BUTTON_H_

#include "ns_signal_emitter.h"
#include "ns_signals.h"
#include "timer.h"

template<uint8_t pin>
class PushButton : public SignalEmitter {
  public:
    PushButton()
      : m_emittingSignal(SIG_NO_SIGNAL)
      , m_wasPushed(false)
    {
    }

    void setup()
    {
      pinMode(pin, INPUT_PULLUP);
    }

    void sync() override
    {
      boolean pushed = !digitalRead(pin);
      Signal signal = SIG_NO_SIGNAL;

      if (pushed != m_wasPushed) {
        m_wasPushed = pushed;
        if (pushed) {
          m_timer.start();
        } else {
          signal |= SIG_BUTTON_PUSHED | signalAccordingToTimePassed();
        }
      }

      if (pushed) {
        signal |= signalAccordingToTimePassed();
      }

      if (m_emittingSignal == SIG_NO_SIGNAL) {
        m_emittingSignal = signal;
      }
    }

    Signal signals() override
    {
      return m_emittingSignal;
    }

    void setSignalsHandled() override
    {
      m_emittingSignal = SIG_NO_SIGNAL;
    }

  private:
    Signal signalAccordingToTimePassed()
    {
      uint32_t timePassed = m_timer.timePassed();
      if (timePassed < 1000) return SIG_PAUSE;
      if (timePassed < 3000) return SIG_TIMER_RESET;
      if (timePassed < 5000) return SIG_HARD_RESET;
      if (timePassed < 10000) return SIG_SHUTDOWN;
      return SIG_FULL_RESET;
    }

    Timer m_timer;
    Signal m_emittingSignal;
    boolean m_wasPushed;
};


#endif // ARDUINO_POMODORO_SIGNAL_EMITTER_PUSH_BUTTON_H_
