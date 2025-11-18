#ifndef ARDUINO_POMODORO_LCD_TIMER_H_
#define ARDUINO_POMODORO_LCD_TIMER_H_

#include "time.h"
#include "timer_action.h"
#include "sync_array.h"

#include <LiquidCrystal_I2C.h>

#define POMO_TIMER_TEXT_COLON ':'
#define POMO_TIMER_TEXT_SIZE 5
#define POMO_TIMER_TEXT_Y_POS 0

static const char* POMO_CYCLES_TEXT = "cycles:";
#define POMO_CYCLES_TEXT_Y_POS 1
#define POMO_CYCLES_MAX_COUNT 1000

#define POMO_POMODORO_MAX_COUNT 4
#define POMO_POMODORO_TEXT_Y_POS 1
byte POMO_CUSTOM_CHAR_X[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b01010,
	0b00100,
	0b01010,
	0b00000,
	0b00000
};
#define POMO_CUSTOM_CHAR_X_IDX 0
byte POMO_CUSTOM_CHAR_POMODORO[8] = {
  0b00000,
	0b00000,
	0b00011,
	0b01110,
	0b01010,
	0b01110,
	0b00000,
	0b00000
};
#define POMO_CUSTOM_CHAR_POMODORO_IDX 1

class LcdTimer {
  public:
    using LcdTimerTimerAction = TimerAction<void(LcdTimer::*)(), LcdTimer*>;

    LcdTimer(int i2c_addr, int width, int height) 
      : m_lcd(i2c_addr, width, height)
      , m_startTimerTextPos(width / 2 - POMO_TIMER_TEXT_SIZE / 2 - 1)
      , m_startCyclesTextPos(0)
      , m_startPomodoroTextPos(width - POMO_POMODORO_MAX_COUNT)
      , m_time(Time { 15, 0 })

      , m_dotsVisible(true)

      , m_timers(
          LcdTimerTimerAction(1000, this, &LcdTimer::decrementTimer),
          LcdTimerTimerAction(200, this, &LcdTimer::blinkDots))
      // , m_decrementTimer(1000, this, &LcdTimer::decrementTimer)
    {
    }
    void setup() {
      m_lcd.init();
      m_lcd.backlight();
      m_lcd.clear();

      m_lcd.createChar(POMO_CUSTOM_CHAR_X_IDX, POMO_CUSTOM_CHAR_X);
      m_lcd.createChar(POMO_CUSTOM_CHAR_POMODORO_IDX, POMO_CUSTOM_CHAR_POMODORO);
    }

    void sync() {
      m_timers.sync();
    }

    void drawTimer() {
      m_lcd.setCursor(m_startTimerTextPos, POMO_TIMER_TEXT_Y_POS);
      printTimer();
    }
    void drawCycles() {
      m_lcd.setCursor(m_startCyclesTextPos, POMO_CYCLES_TEXT_Y_POS);
      m_lcd.print(POMO_CYCLES_TEXT);
      m_lcd.print(m_cycles % POMO_CYCLES_MAX_COUNT);
    }
    void drawPomodoro() {
      m_lcd.setCursor(m_startPomodoroTextPos, POMO_POMODORO_TEXT_Y_POS);
      for (int i = 0; i < POMO_POMODORO_MAX_COUNT; ++i) {
        m_lcd.write((byte)POMO_CUSTOM_CHAR_X_IDX);
      }
    }

  private:
    void printTimer() {
      printWithPadding(m_time.minutes, ' ');
      if (m_dotsVisible) {
        m_lcd.print(POMO_TIMER_TEXT_COLON);
      } else {
        m_lcd.print(' ');
      }
      printWithPadding(m_time.seconds, '0');
    }
    void printWithPadding(uint8_t n, char padding) {
      if (n >= 10 || n < 0) {
        m_lcd.print(n % 100);
      } else {
        m_lcd.print(padding);
        m_lcd.print(n);
      }
    }

    void decrementTimer() {
      --m_time;
      drawTimer();
    }

    void blinkDots() {
      m_dotsVisible = !m_dotsVisible;
      drawTimer();
    }

    LiquidCrystal_I2C m_lcd;
    int m_startTimerTextPos;
    int m_startCyclesTextPos;
    int m_startPomodoroTextPos;
    Time m_time;
    uint8_t m_cycles;

    boolean m_dotsVisible;

    // LcdTimerTimerAction m_decrementTimer;
    SyncArray<LcdTimerTimerAction, 2> m_timers;
};

#endif // ARDUINO_POMODORO_LCD_TIMER_H_
