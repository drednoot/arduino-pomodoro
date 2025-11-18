#ifndef ARDUINO_POMODORO_MOVE_H_
#define ARDUINO_POMODORO_MOVE_H_

namespace pomo {

template<typename T>
T&& move(T val) { return static_cast<T&&>(val); }

} // namespace ns

#endif // ARDUINO_POMODORO_MOVE_H_
