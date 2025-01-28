#include <ir.h>

static enum MODE mode = IRSTART;

void set_mode(enum MODE new_mode) {
  mode = new_mode;
}

enum MODE get_mode(void) {
  return mode;
}

void init_ir(PROTOCOL protocol) {
  switch (protocol) {
    case RC5:
      rc5_init();
      break;
    case NEC:
      nec_init();
      break;
    case SIRC:
      sirc_init();
      break;
    default:
      error_led();
      break;
  }
}

void send_ready(PROTOCOL protocol) {
  switch (mode) {
    case IRSTART:
      switch (protocol) {
        case RC5:
          rc5_send_prog();
          break;
        case NEC:
          nec_send_ready();
          break;
        case SIRC:
          sirc_send_ready();
          break;
        default:
          error_led();
          break;
      }
      break;
    case IRMENU:
      switch (protocol) {
        case RC5:
          rc5_send_menu_mode();
          break;
        default:
          error_led();
          break;
      }
      break;
  }
}

void send_start(PROTOCOL protocol) {

  switch (mode) {
    case IRSTART:
      switch (protocol) {
        case RC5:
          rc5_send_start();
          break;
        case NEC:
          nec_send_start();
          break;
        case SIRC:
          sirc_send_start();
          break;
        default:
          error_led();
          break;
      }
      break;
    case IRMENU:
      switch (protocol) {
        case RC5:
          rc5_send_menu_up();
          break;
        default:
          error_led();
          break;
      }
      break;
  }
}

void send_stop(PROTOCOL protocol) {
  switch (mode) {
    case IRSTART:
      switch (protocol) {
        case RC5:
          rc5_send_stop();
          break;
        case NEC:
          nec_send_stop();
          break;
        case SIRC:
          sirc_send_stop();
          break;
        default:
          error_led();
          break;
      }
      break;
    case IRMENU:
      switch (protocol) {
        case RC5:
          rc5_send_menu_down();
          break;
        default:
          error_led();
          break;
      }
      break;
  }
}