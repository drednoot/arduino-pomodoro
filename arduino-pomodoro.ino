#include "lcd_timer.h"

LcdTimer lcd_timer(0x27, 16, 2);

void setup() {
  lcd_timer.setup();
  lcd_timer.drawTimer();
  lcd_timer.drawCycles();
  lcd_timer.drawPomodoro();
}

void loop() {

}
