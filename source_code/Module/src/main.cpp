#include "control.h"
#include "pinout.h"
#include "rc5.h"
#include <Arduino.h>

bool led_state = false;

void setup(void) {
  init_pinout();
  rc5_init();
}

void loop(void) {
  if (module_is_started()) {
    digitalWrite(PIN_LED, HIGH);
  } else {
    digitalWrite(PIN_LED, led_state = !led_state);
    uint16_t count = 0;
    while (((led_state && count <= 100) || (!led_state && count <= 1600)) && !module_is_started()) {
      count++;
      delay(1);
    }
    // if (!digitalRead(PIN_RESET)) {
    //   rc5_reset_eeprom();
    // }
  }
}