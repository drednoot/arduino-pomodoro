#ifndef ARDUINO_POMODORO_NS_TASK_H_
#define ARDUINO_POMODORO_NS_TASK_H_

#include "timer_action.h"

class Task {
  public:
    virtual void setup() = 0;
    virtual void sync() = 0;
    virtual bool isDone() const = 0;
};

#endif // ARDUINO_POMODORO_NS_TASK_H_
