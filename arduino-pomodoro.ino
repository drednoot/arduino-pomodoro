#include "ns_kernel.h"

Kernel<2> kernel;

void setup() {
  Serial.begin(9600);

  kernel.setup();
}

void loop() {
  kernel.sync();
}
