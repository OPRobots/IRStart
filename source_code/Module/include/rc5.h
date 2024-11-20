#ifndef RC5_H
#define RC5_H

#include "control.h"
#include "pinout.h"
#include <Arduino.h>
#include <EEPROM.h>
#include <stdint.h>

#define EEPROM_SIZE 512
enum EEPROM_ADDR {
  EEPROM_ADDR_RC5_START = 0,
  EEPROM_ADDR_RC5_STOP = 16
};

enum RC5_TRIGGER {
  RC5_TRIGGER_FALLING,
  RC5_TRIGGER_RISING,
};

#define MIN_SHORT 444
#define MAX_SHORT 1333
#define MIN_LONG 1334
#define MAX_LONG 2222

#define STATE_START1 0
#define STATE_MID1 1
#define STATE_MID0 2
#define STATE_START0 3

#define S2_MASK 0x1000
#define S2_SHIFT 12
#define TOGGLE_MASK 0x0800
#define TOGGLE_SHIFT 11
#define ADDRESS_MASK 0x7C0
#define ADDRESS_SHIFT 6
#define COMMAND_MASK 0x003F
#define COMMAND_SHIFT 0

#define ADDRESS_PROG 0x0B
#define ADDRESS_COMP 0x07

#define DATA_STOP 0
#define DATA_START 1

#define RC5_DATA_LENGTH 2

void rc5_init(void);
void rc5_register(enum RC5_TRIGGER trigger);
void rc5_isr(void);
void rc5_update(void);
// void rc5_reset_eeprom(void);

#endif