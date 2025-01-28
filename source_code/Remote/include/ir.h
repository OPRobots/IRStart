#ifndef IR_H
#define IR_H

#include <ir.h>

#include <nec.h>
#include <rc5.h>
#include <sirc.h>

enum PROTOCOL {
  RC5,
  NEC,
  SIRC,
};

#define NUM_MODES 2
enum MODE {
  IRSTART = 0,
  IRMENU = 1,
};

void set_mode(enum MODE new_mode);

enum MODE get_mode(void);

void init_ir(PROTOCOL protocol);

void send_ready(PROTOCOL protocol);

void send_start(PROTOCOL protocol);

void send_stop(PROTOCOL protocol);

#endif