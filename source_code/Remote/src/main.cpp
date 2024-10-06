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
  set_rgb(0, 50, 0, 0);
  set_rgb(1, 0, 50, 0);
  set_rgb(2, 0, 0, 50);
}

void loop() {
  // debug_all();
  while (get_btn_ready()) {
    if (digitalRead(DIP_SW_3)) {
      send_menu_mode(protocol);
    } else {
      send_ready(protocol);
    }
  }
  while (get_btn_start()) {
    if (digitalRead(DIP_SW_3)) {
      send_menu_up(protocol);
    } else {
      send_start(protocol);
    }
  }
  while (get_btn_stop()) {
    if (digitalRead(DIP_SW_3)) {
      send_menu_down(protocol);
    } else {
      send_stop(protocol);
    }
  }
}
