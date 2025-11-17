#ifndef ARDUINO_POMODORO_TIME_H_
#define ARDUINO_POMODORO_TIME_H_

#define SECONDS_IN_MINUTE 60

struct Time {
  uint8_t minutes;
  uint8_t seconds;

  Time& operator--()
  {
    if (seconds == 0) {
      seconds = SECONDS_IN_MINUTE - 1;
      if (minutes != 0) {
        --minutes;
      }
    } else {
      --seconds;
    }

    return *this;
  }

  operator boolean() const { return minutes > 0 && seconds > 0; }
};

#endif // ARDUINO_POMODORO_TIME_H_
