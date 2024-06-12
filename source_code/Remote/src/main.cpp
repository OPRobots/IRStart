#include <Arduino.h>

#include <config.h>
#include <debug.h>
#include <io.h>
#include <ir.h>

PROTOCOL protocol = RC5;

void setup() {
  init_components();
  if (get_btn_start()) {
    protocol = RC5;
  } else if (get_btn_stop()) {
    protocol = NEC;
  } else if (get_btn_ready()) {
    protocol = SIRC;
  }
  init_ir(protocol);
  while (get_btn_ready() || get_btn_start() || get_btn_stop()) {
   blink_led(500);
  }
}

void loop() {
  // debug_all();
  while (get_btn_ready()) {
    send_ready(protocol);
  }
  while (get_btn_start()) {
    send_start(protocol);
  }
  while (get_btn_stop()) {
    send_stop(protocol);
  }
  set_led(false);
  blink_led(500);
  // delay(100);
}
