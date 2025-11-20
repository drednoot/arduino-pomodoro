#ifndef ARDUINO_POMODORO_NS_SIGNAL_EMITTER_H_
#define ARDUINO_POMODORO_NS_SIGNAL_EMITTER_H_

#include "ns_signals.h"

class SignalEmitter {
  public:
    virtual void sync() = 0;
    virtual Signal signals() = 0;
    virtual void setSignalsHandled() = 0;
};

#endif // ARDUINO_POMODORO_NS_SIGNAL_EMITTER_H_
