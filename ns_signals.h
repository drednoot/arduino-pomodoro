#ifndef ARDUINO_POMODORO_SIGNALS_H_
#define ARDUINO_POMODORO_SIGNALS_H_

enum Signal {
  SIG_NO_SIGNAL = 0x0,
  SIG_PAUSE = 0x1, // timer pause
  SIG_TIMER_RESET = 0x2, // timer reset
  SIG_HARD_RESET = 0x4, // cycles, pomodoro reset
  SIG_SHUTDOWN = 0x8, // arduino sleep
  SIG_FULL_RESET = 0x10, // arduino restart
  SIG_ACTIVATE = 0x20, // button pushed

  SIG_MIN = SIG_PAUSE,
  SIG_MAX = SIG_ACTIVATE,
};

using Signals = uint16_t;

#endif // ARDUINO_POMODORO_SIGNALS_H_
