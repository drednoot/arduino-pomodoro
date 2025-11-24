#include "ns_kernel.h"

Kernel kernel;

void setup() {
  kernel.setup();
}

void loop() {
  kernel.sync();
}
