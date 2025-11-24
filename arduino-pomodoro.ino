#include "ns_kernel.h"

#include <GyverPower.h>

Kernel kernel;

void setup() {
  kernel.setup();

  power.hardwareDisable(PWR_TIMER1 | PWR_UART0 | PWR_SPI | PWR_ADC);
  power.setSleepMode(POWERDOWN_SLEEP);
}

void loop() {
  kernel.sync();
}
