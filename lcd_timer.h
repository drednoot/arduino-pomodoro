#ifndef ARDUINO_POMODORO_LCD_TIMER_H_
#define ARDUINO_POMODORO_LCD_TIMER_H_

#include "time.h"

#include <LiquidCrystal_I2C.h>

#define POMO_TIMER_TEXT_COLON ':'
#define POMO_TIMER_TEXT_SIZE 5
#define POMO_TIMER_TEXT_Y_POS 0

static const char* POMO_CYCLES_TEXT = "cycles:";
#define POMO_CYCLES_TEXT_SIZE 10
#define POMO_CYCLES_TEXT_Y_POS 1

static const char* POMO_WORK_TEXT = "work";
#define POMO_WORK_TEXT_SIZE 4
static const char* POMO_REST_TEXT = "rest";
#define POMO_REST_TEXT_SIZE 4
#define POMO_WORK_TEXT_Y_POS 0
#define POMO_REST_TEXT_Y_POS 0

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
    LcdTimer(int i2c_addr, int width, int height, Time defaultTime = Time {25, 0}) 
      : m_lcd(i2c_addr, width, height)
      , m_startTimerTextPos(width / 2 - POMO_TIMER_TEXT_SIZE / 2 - 1)
      , m_startCyclesTextPos(0)
      , m_startPomodoroTextPos(width - POMO_POMODORO_MAX_COUNT)

      , m_time(defaultTime)
      , m_dotsVisible(true)
      , m_cycles(0)
      , m_pomodoro(0)
      , m_isWork(true)
      , m_timerVisible(true)
      , m_textVisible(true)
    {
    }
    void setup()
    {
      m_lcd.init();
      m_lcd.backlight();
      m_lcd.clear();

      m_lcd.createChar(POMO_CUSTOM_CHAR_X_IDX, POMO_CUSTOM_CHAR_X);
      m_lcd.createChar(POMO_CUSTOM_CHAR_POMODORO_IDX, POMO_CUSTOM_CHAR_POMODORO);

      drawScreen();
    }

    void setTime(Time time)
    {
      m_time = time;
      drawTimer();
    }

    void setDotsVisible(boolean isVisible)
    {
      m_dotsVisible = isVisible;
      drawTimer();
    }

    void setTimerVisible(boolean isVisible)
    {
      m_timerVisible = isVisible;
      drawTimer();
    }

    void setTextVisible(boolean isVisible)
    {
      m_textVisible = isVisible;
      drawScreen();
    }

    void setBacklightEnabled(boolean isEnabled)
    {
      if (isEnabled) {
        m_lcd.backlight();
      } else {
        m_lcd.noBacklight();
      }
    }

    void setIsWork(boolean isWork)
    {
      m_isWork = isWork;
      drawIsWork();
    }

  private:
    void drawScreen()
    {
      drawTimer();
      drawCycles();
      drawPomodoro();
      drawIsWork();
    }

    void drawTimer()
    {
      m_lcd.setCursor(m_startTimerTextPos, POMO_TIMER_TEXT_Y_POS);
      printTimer();
    }
    void printTimer()
    {
      if (!m_timerVisible || !m_textVisible) {
        for (int i = 0; i < POMO_TIMER_TEXT_SIZE; ++i) {
          m_lcd.print(' ');
        }
        return;
      }

      printWithPadding(m_time.minutes, ' ');
      if (m_dotsVisible) {
        m_lcd.print(POMO_TIMER_TEXT_COLON);
      } else {
        m_lcd.print(' ');
      }
      printWithPadding(m_time.seconds, '0');
    }
    void printWithPadding(uint8_t n, char padding)
    {
      if (n >= 10) {
        m_lcd.print(n % 100);
      } else {
        m_lcd.print(padding);
        m_lcd.print(n);
      }
    }

    void drawCycles()
    {
      m_lcd.setCursor(m_startCyclesTextPos, POMO_CYCLES_TEXT_Y_POS);

      if (!m_textVisible) {
        for (int i = 0; i < POMO_CYCLES_TEXT_SIZE; ++i) {
          m_lcd.print(' ');
        }
        return;
      }

      m_lcd.print(POMO_CYCLES_TEXT);
      m_lcd.print(m_cycles);
    }

    void drawPomodoro()
    {
      m_lcd.setCursor(m_startPomodoroTextPos, POMO_POMODORO_TEXT_Y_POS);

      if (!m_textVisible) {
        for (int i = 0; i < POMO_POMODORO_MAX_COUNT; ++i) {
          m_lcd.print(' ');
        }
        return;
      }

      for (int i = 0; i < m_pomodoro; ++i) {
        m_lcd.write((byte)POMO_CUSTOM_CHAR_POMODORO_IDX);
      }
      for (int i = 0; i < POMO_POMODORO_MAX_COUNT - m_pomodoro; ++i) {
        m_lcd.write((byte)POMO_CUSTOM_CHAR_X_IDX);
      }
    }

    void drawIsWork()
    {
      if (m_isWork) {
        m_lcd.setCursor(0, POMO_WORK_TEXT_Y_POS);

        if (!m_textVisible) {
          for (int i = 0; i < POMO_WORK_TEXT_SIZE; ++i) {
            m_lcd.print(' ');
          }
          return;
        }

        m_lcd.print(POMO_WORK_TEXT);
      } else {
        m_lcd.setCursor(0, POMO_REST_TEXT_Y_POS);

        if (!m_textVisible) {
          for (int i = 0; i < POMO_REST_TEXT_SIZE; ++i) {
            m_lcd.print(' ');
          }
          return;
        }

        m_lcd.print(POMO_REST_TEXT);
      }
    }

    LiquidCrystal_I2C m_lcd;
    int m_startTimerTextPos;
    int m_startCyclesTextPos;
    int m_startPomodoroTextPos;

    Time m_time;
    boolean m_dotsVisible;
    uint8_t m_cycles;
    uint8_t m_pomodoro;
    boolean m_isWork;
    boolean m_timerVisible;
    boolean m_textVisible;
};

#endif // ARDUINO_POMODORO_LCD_TIMER_H_
