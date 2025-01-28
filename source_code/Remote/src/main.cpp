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
    send_ready(protocol);
  }
  while (get_btn_start()) {
    send_start(protocol);
  }
  while (get_btn_stop()) {
    send_stop(protocol);
  }


  if (get_btn_mode()) {
    while (get_btn_mode()) {
    }
    set_mode((MODE)((get_mode() + 1) % NUM_MODES));
  }

  switch (get_mode()) {
    case IRSTART:
      set_rgb(0, 0, 50, 0);
      set_rgb(1, 50, 0, 0);
      set_rgb(2, 50, 0, 50);
      break;
    case IRMENU:
      set_rgb(0, 0, 0, 0);
      set_rgb(1, 0, 0, 0);
      set_rgb(2, 0, 0, 50);
      break;
    default:
      set_rgb(0, 50, 0, 0);
      set_rgb(1, 50, 0, 0);
      set_rgb(2, 50, 0, 0);
      break;
  }
}
