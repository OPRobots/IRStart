/**
    Basado en https://clearwater.com.au/code/rc5
 */
#include "rc5.h"

enum RC5_EVENT {
  EVENT_SHORTSPACE = 0,
  EVENT_SHORTPULSE = 2,
  EVENT_LONGSPACE = 4,
  EVENT_LONGPULSE = 6
};

static uint8_t rc5_stored_data[RC5_DATA_LENGTH];

static const uint8_t trans[] = {0x01, 0x91, 0x9B, 0xFB};

static uint8_t state = STATE_MID1;
static uint16_t cmd = 1;
static uint8_t bits = 1;
static uint32_t last_us = 0;

static void rc5_manage_command(uint16_t message) {
  // unsigned char toggle = (message & TOGGLE_MASK) >> TOGGLE_SHIFT;
  uint8_t address = (message & ADDRESS_MASK) >> ADDRESS_SHIFT;
  uint8_t command = (message & COMMAND_MASK) >> COMMAND_SHIFT;
  switch (address) {
    case ADDRESS_PROG:
      rc5_stored_data[0] = command;
      rc5_stored_data[1] = command + 1;
      EEPROM.write(EEPROM_ADDR_RC5_START, rc5_stored_data[DATA_START]);
      EEPROM.write(EEPROM_ADDR_RC5_STOP, rc5_stored_data[DATA_STOP]);
      module_feedback();
      break;
    case ADDRESS_COMP:
      if (command == rc5_stored_data[DATA_START]) {
        module_start();
      } else if (command == rc5_stored_data[DATA_STOP]) {
        module_stop();
      }
      break;
    default:
      if (command == 0x01) {
        module_start();
      } else if (command == 0x02) {
        module_stop();
      }
      break;
  }
}

static void reset(void) {
  state = STATE_MID1;
  cmd = 1;
  bits = 1;
  last_us = micros();
}

static void rc5_decode_event(enum RC5_EVENT event) {

  uint8_t newState = (trans[state] >> event) & 0x3;
  if (newState == state) {
    reset();
  } else {
    state = newState;
    if (newState == STATE_MID0) {
      cmd = (cmd << 1);
      bits++;
    } else if (newState == STATE_MID1) {
      cmd = (cmd << 1) + 1;
      bits++;
    }
  }
}

static void rc5_decode_pulse(enum RC5_TRIGGER trigger, uint32_t elapsed) {
  if (elapsed >= MIN_SHORT && elapsed <= MAX_SHORT) {
    rc5_decode_event(trigger == RC5_TRIGGER_FALLING ? EVENT_SHORTSPACE : EVENT_SHORTPULSE);
  } else if (elapsed >= MIN_LONG && elapsed <= MAX_LONG) {
    rc5_decode_event(trigger == RC5_TRIGGER_FALLING ? EVENT_LONGSPACE : EVENT_LONGPULSE);
  } else {
    reset();
  }
}

void rc5_init(void) {
  rc5_stored_data[DATA_START] = EEPROM.read(EEPROM_ADDR_RC5_START);
  rc5_stored_data[DATA_STOP] = EEPROM.read(EEPROM_ADDR_RC5_STOP);
}

// void rc5_reset_eeprom(void) {
//   EEPROM.write(EEPROM_ADDR_RC5_START, 0);
//   EEPROM.write(EEPROM_ADDR_RC5_STOP, 0);
//   rc5_init();
//   module_feedback();
//   // use_home = true;
// }

void rc5_register(enum RC5_TRIGGER trigger) {
  rc5_decode_pulse(trigger, micros() - last_us);
  last_us = micros();

  delayMicroseconds(100);

  if (bits == 14) {
    rc5_manage_command(cmd);
    cmd = 0;
    bits = 0;
  }
}

void rc5_isr(void) {
  rc5_register(digitalRead(PIN_IR) ? RC5_TRIGGER_RISING : RC5_TRIGGER_FALLING);
}

/*void rc5_update(void) {
  bool new_pin_state = digitalRead(PIN_IR);
  if (new_pin_state != pin_state) {
    pin_state = new_pin_state;
    rc5_register(pin_state ? RC5_TRIGGER_RISING : RC5_TRIGGER_FALLING);
  }
}*/