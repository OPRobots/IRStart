#include <ir.h>

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
}

void send_start(PROTOCOL protocol) {
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
}

void send_stop(PROTOCOL protocol) {
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
}