#ifndef ARDUINO_POMODORO_LCD_TIMER_H_
#define ARDUINO_POMODORO_LCD_TIMER_H_

#include "time.h"

#ifdef ADAFRUIT_SSD1306
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#else
#include <LiquidCrystal_I2C.h>
#endif

#ifdef ADAFRUIT_SSD1306
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define TEXT_SIZE 2
#define PIXEL_TO_OFFSET_X_RATIO (8 * TEXT_SIZE)
#define PIXEL_TO_OFFSET_Y_RATIO (10 * TEXT_SIZE)
#else
#define PIXEL_TO_OFFSET_X_RATIO 1
#define PIXEL_TO_OFFSET_Y_RATIO 1
#endif

#define POMO_TIMER_TEXT_COLON ':'
#define POMO_TIMER_TEXT_SIZE 5
#define POMO_TIMER_TEXT_Y_POS 1

static const char* POMO_CYCLES_TEXT = "cycles:";
#define POMO_CYCLES_TEXT_SIZE 10
#define POMO_CYCLES_TEXT_Y_POS 2

static const char* POMO_WORK_TEXT = "work";
#define POMO_WORK_TEXT_SIZE 4
static const char* POMO_REST_TEXT = "rest";
#define POMO_REST_TEXT_SIZE 4
#define POMO_WORK_TEXT_Y_POS 0
#define POMO_REST_TEXT_Y_POS 0

#define POMO_POMODORO_MAX_COUNT 4
#define POMO_POMODORO_TEXT_Y_POS 0
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
    LcdTimer(int i2c_addr, uint8_t width, uint8_t height, Time defaultTime = Time {25, 0}) 
#ifdef ADAFRUIT_SSD1306
      : m_lcd(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1)
      , m_i2cAddr(i2c_addr)
#else
      : m_lcd(i2c_addr, width, height)
#endif
      , m_startTimerTextPos(width / 2 - POMO_TIMER_TEXT_SIZE / 2 - 1)
      , m_startCyclesTextPos(0)
      , m_startPomodoroTextPos(width - POMO_POMODORO_MAX_COUNT)
      , m_width(width)
      , m_height(height)

      , m_time(defaultTime)
      , m_dotsVisible(true)
      , m_cycles(0)
      , m_pomodoro(1)
      , m_isWork(true)
      , m_timerVisible(true)
      , m_textVisible(true)
    {
    }
    void setup()
    {
#ifdef ADAFRUIT_SSD1306
      m_lcd.begin(SSD1306_SWITCHCAPVCC, m_i2cAddr);
      m_lcd.setTextSize(TEXT_SIZE);
      m_lcd.setTextColor(SSD1306_WHITE);
#else
      m_lcd.init();

      setBacklightEnabled(true);

      m_lcd.createChar(POMO_CUSTOM_CHAR_X_IDX, POMO_CUSTOM_CHAR_X);
      m_lcd.createChar(POMO_CUSTOM_CHAR_POMODORO_IDX, POMO_CUSTOM_CHAR_POMODORO);
#endif
      clear();

      drawScreen();
    }

    void setTime(Time time)
    {
      m_time = time;
      drawTimer();
    }

    void setCycles(uint8_t cycles)
    {
      m_cycles = cycles;
      drawCycles();
    }

    uint8_t cycles() const
    {
      return m_cycles;
    }

    void setPomodoro(uint8_t pomodoro)
    {
      m_pomodoro = pomodoro;
      drawPomodoro();
    }

    uint8_t pomodoro() const
    {
      return m_pomodoro;
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
#ifdef ADAFRUIT_SSD1306
      (void)isEnabled;
#else
      if (isEnabled) {
        m_lcd.backlight();
      } else {
        m_lcd.noBacklight();
      }
#endif
    }

    void setDisplayEnabled(boolean isEnabled)
    {
#ifdef ADAFRUIT_SSD1306
      if (isEnabled) {
        m_lcd.ssd1306_command(SSD1306_DISPLAYON);
      } else {
        m_lcd.ssd1306_command(SSD1306_DISPLAYOFF);
      }
#else
      if (isEnabled) {
        m_lcd.display();
      } else {
        m_lcd.noDisplay();
      }
#endif
    }

    void setIsWork(boolean isWork)
    {
      m_isWork = isWork;
      drawIsWork();
    }

    void setPower(boolean isOn)
    {
      setDisplayEnabled(isOn);
      setBacklightEnabled(isOn);
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
      setCursor(m_startTimerTextPos, POMO_TIMER_TEXT_Y_POS);
      printTimer();
      upload();
    }
    void printTimer()
    {
      if (!m_timerVisible || !m_textVisible) {
        for (int i = 0; i < POMO_TIMER_TEXT_SIZE; ++i) {
          print(' ');
        }
        return;
      }

      printWithPadding(m_time.minutes, ' ');
      if (m_dotsVisible) {
        print(POMO_TIMER_TEXT_COLON);
      } else {
        print(' ');
      }
      printWithPadding(m_time.seconds, '0');
    }
    void printWithPadding(uint8_t n, char padding)
    {
      if (n >= 10) {
        print(n % 100);
      } else {
        print(padding);
        print(n);
      }
    }

    void drawCycles()
    {
      setCursor(m_startCyclesTextPos, POMO_CYCLES_TEXT_Y_POS);

      if (!m_textVisible) {
        for (int i = 0; i < POMO_CYCLES_TEXT_SIZE; ++i) {
          print(' ');
        }
        return;
      }

      print(POMO_CYCLES_TEXT);
      print(m_cycles);
      upload();
    }

    void drawPomodoro()
    {
      setCursor(m_startPomodoroTextPos, POMO_POMODORO_TEXT_Y_POS);

      if (!m_textVisible) {
        for (int i = 0; i < POMO_POMODORO_MAX_COUNT; ++i) {
          print(' ');
        }
        return;
      }

      for (int i = 0; i < m_pomodoro; ++i) {
        // m_lcd.write((byte)POMO_CUSTOM_CHAR_POMODORO_IDX);
      }
      for (int i = 0; i < POMO_POMODORO_MAX_COUNT - m_pomodoro; ++i) {
        // m_lcd.write((byte)POMO_CUSTOM_CHAR_X_IDX);
      }
      upload();
    }

    void drawIsWork()
    {
      if (m_isWork) {
        setCursor(0, POMO_WORK_TEXT_Y_POS);

        if (!m_textVisible) {
          for (int i = 0; i < POMO_WORK_TEXT_SIZE; ++i) {
            print(' ');
          }
          return;
        }

        print(POMO_WORK_TEXT);
      } else {
        setCursor(0, POMO_REST_TEXT_Y_POS);

        if (!m_textVisible) {
          for (int i = 0; i < POMO_REST_TEXT_SIZE; ++i) {
            print(' ');
          }
          return;
        }

        print(POMO_REST_TEXT);
      }
      upload();
    }

    void clear()
    {
#ifdef ADAFRUIT_SSD1306
      m_lcd.clearDisplay();
#else
      m_lcd.clear();
#endif
    }

    void upload()
    {
#ifdef ADAFRUIT_SSD1306
      m_lcd.display();
#endif
    }

    void setCursor(uint8_t x, uint8_t y)
    {
      m_lcd.setCursor(x * PIXEL_TO_OFFSET_X_RATIO, y * PIXEL_TO_OFFSET_Y_RATIO);
    }

    void print(char ch)
    {
#ifdef ADAFRUIT_SSD1306
      m_lcd.fillRect(
          m_lcd.getCursorX(), m_lcd.getCursorY(),
          PIXEL_TO_OFFSET_X_RATIO, PIXEL_TO_OFFSET_Y_RATIO,
          SSD1306_BLACK);
#endif
      m_lcd.print(ch);
    }

    void print(const char* ch)
    {
      while (*ch) {
        print(*ch);
        ++ch;
      }
    }

    void print(int dig)
    {
      print(static_cast<char>('0' + dig));
    }

#ifdef ADAFRUIT_SSD1306
    Adafruit_SSD1306 m_lcd;
    int m_i2cAddr;
#else
    LiquidCrystal_I2C m_lcd;
#endif
    int m_startTimerTextPos;
    int m_startCyclesTextPos;
    int m_startPomodoroTextPos;
    uint8_t m_width;
    uint8_t m_height;

    Time m_time;
    boolean m_dotsVisible;
    uint8_t m_cycles;
    uint8_t m_pomodoro;
    boolean m_isWork;
    boolean m_timerVisible;
    boolean m_textVisible;
};

#endif // ARDUINO_POMODORO_LCD_TIMER_H_
