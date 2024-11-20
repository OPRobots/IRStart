#include "pinout.h"

void init_pinout(void) {
  pinMode(PIN_IR, INPUT);
  pinMode(PIN_SIGNAL, OUTPUT);
  pinMode(PIN_LED, OUTPUT);
  // pinMode(PIN_RESET, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(PIN_IR), rc5_isr, CHANGE);
}