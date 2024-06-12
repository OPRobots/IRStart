#include <nec.h>

#define PWM_FREQ 37000
#define PWM_RES 10
#define PWM_DUTY 1024 * 1 / 3

#define ADDR 0x01

#define CMD_READY 0x0
#define CMD_START 0x1
#define CMD_STOP 0x2

#define CMD_WAIT_MS 110

static long last_cmd_ms = 0;
static unsigned char last_cmd = 0;

static void ir_on() {
  ledcWrite(IR_CMD_PWM, PWM_DUTY);
}

static void ir_off() {
  ledcWrite(IR_CMD_PWM, 0);
}

static void send_one() {
  ir_on();
  delayMicroseconds(560);
  ir_off();
  delayMicroseconds(1690);
}

static void send_zero() {
  ir_on();
  delayMicroseconds(560);
  ir_off();
  delayMicroseconds(560);
}

static void send_start() {
  ir_on();
  delayMicroseconds(9000);
  ir_off();
  delayMicroseconds(4500);
}

static void send_end() {
  ir_on();
  delayMicroseconds(560);
  ir_off();
}

static void send_repeat() {
  ir_on();
  delayMicroseconds(9000);
  ir_off();
  delayMicroseconds(2250);
  ir_on();
  delayMicroseconds(560);
  ir_off();
}

static void send_packet(unsigned char addr, unsigned char cmd) {
  if (millis() > last_cmd_ms + CMD_WAIT_MS) {
    if (last_cmd == cmd && last_cmd_ms != 0 && millis() - last_cmd_ms < CMD_WAIT_MS + 50) {
      last_cmd_ms = millis();
      send_repeat();
      return;
    } else {
      last_cmd = cmd;
    }
    last_cmd_ms = millis();
    send_start();

    for (int i = 0; i < 8; i++) {
      if (bitRead(addr, 8 - i)) {
        send_one();
      } else {
        send_zero();
      }
    }
    for (int i = 0; i < 8; i++) {
      if (bitRead(addr, 8 - i)) {
        send_zero();
      } else {
        send_one();
      }
    }

    for (int i = 0; i < 8; i++) {
      if (bitRead(cmd, 7 - i)) {
        send_one();
      } else {
        send_zero();
      }
    }
    for (int i = 0; i < 8; i++) {
      if (bitRead(cmd, 7 - i)) {
        send_zero();
      } else {
        send_one();
      }
    }

    send_end();
  }
}

void nec_init() {
  ledcSetup(IR_CMD_PWM, PWM_FREQ, 10);
  ledcAttachPin(IR_CMD, IR_CMD_PWM);
  ledcWrite(IR_CMD_PWM, 0);
}

void nec_send_ready() {
  set_led(true);
  send_packet(ADDR, CMD_READY);
  set_led(false);
  delay(30);
}

void nec_send_start() {
  set_led(true);
  send_packet(ADDR, CMD_START);
  set_led(false);
  delay(30);
}

void nec_send_stop() {
  set_led(true);
  send_packet(ADDR, CMD_STOP);
  set_led(false);
  delay(30);
}