#ifndef ARDUINO_POMODORO_SIGNALS_H_
#define ARDUINO_POMODORO_SIGNALS_H_

enum Signal {
  SIG_NO_SIGNAL = 0x0,
  SIG_PAUSE = 0x1, // timer pause
  SIG_TIMER_RESET = 0x2, // timer reset
  SIG_HARD_RESET = 0x4, // cycles, pomodoro reset
  SIG_SHUTDOWN = 0x8, // arduino sleep
  SIG_FULL_RESET = 0x10, // arduino restart
  SIG_BUTTON_PUSHED = 0x20, // button pushed

  SIG_MIN = SIG_PAUSE,
  SIG_MAX = SIG_BUTTON_PUSHED,
};

#endif // ARDUINO_POMODORO_SIGNALS_H_
