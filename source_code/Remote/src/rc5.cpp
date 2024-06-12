#include <rc5.h>

#define PWM_FREQ 36000
#define PWM_RES 10
#define PWM_DUTY 1024 * 1 / 3

#define ADDR 0x07
#define ADDR_PROG 0x0B

#define CMD_READY 0x0
#define CMD_START 0x2
#define CMD_STOP 0x1

static bool last_toggle_bit = false;

static void ir_on(int IR_PWM) {
  ledcWrite(IR_PWM, PWM_DUTY);
}

static void ir_off(int IR_PWM) {
  ledcWrite(IR_PWM, 0);
}

static void send_one(int IR_PWM) {
  ir_off(IR_PWM);
  delayMicroseconds(889);
  ir_on(IR_PWM);
  delayMicroseconds(889);
}

static void send_zero(int IR_PWM) {
  ir_on(IR_PWM);
  delayMicroseconds(889);
  ir_off(IR_PWM);
  delayMicroseconds(889);
}

static void send_start(int IR_PWM) {
  send_one(IR_PWM);
  send_one(IR_PWM);
  if (last_toggle_bit) {
    send_zero(IR_PWM);
  } else {
    send_one(IR_PWM);
  }
  last_toggle_bit = !last_toggle_bit;
}

static void send_packet(int IR_PWM, unsigned char addr, unsigned char cmd) {
  send_start(IR_PWM);

  for (int i = 0; i < 5; i++) {
    if (bitRead(addr, 4 - i)) {
      send_one(IR_PWM);
    } else {
      send_zero(IR_PWM);
    }
  }

  for (int i = 0; i < 6; i++) {
    if (bitRead(cmd, 5 - i)) {
      send_one(IR_PWM);
    } else {
      send_zero(IR_PWM);
    }
  }
  ir_off(IR_PWM);
}

void rc5_init() {
  ledcSetup(IR_CMD_PWM, PWM_FREQ, 10);
  ledcAttachPin(IR_CMD, IR_CMD_PWM);
  ledcWrite(IR_CMD_PWM, 0);

  ledcSetup(IR_PROG_PWM, PWM_FREQ, 10);
  ledcAttachPin(IR_PROG, IR_PROG_PWM);
  ledcWrite(IR_PROG_PWM, 0);
}

void rc5_send_prog() {
  unsigned char id = get_dip_switch();
  set_led(true);
  if (id == 0) {
    send_packet(IR_PROG_PWM, ADDR_PROG, 0x1);
  } else {
    send_packet(IR_PROG_PWM, ADDR_PROG, id << 1);
  }
  set_led(false);
  delay(30);
}

void rc5_send_start() {
  set_led(true);
  unsigned char id = get_dip_switch();
  if (id == 0) {
    send_packet(IR_CMD_PWM, ADDR, CMD_START);
  } else {
    send_packet(IR_CMD_PWM, ADDR, (id << 1) | 0b00000001);
  }
  set_led(false);
  delay(30);
}

void rc5_send_stop() {
  set_led(true);
  unsigned char id = get_dip_switch();
  if (id == 0) {
    send_packet(IR_CMD_PWM, ADDR, CMD_STOP);
  } else {
    send_packet(IR_CMD_PWM, ADDR, (id << 1) & 0b11111110);
  }
  set_led(false);
  delay(30);
}