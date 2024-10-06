#ifndef IR_H
#define IR_H

#include <ir.h>

#include <nec.h>
#include <rc5.h>
#include <sirc.h>

enum PROTOCOL { RC5,
                NEC,
                SIRC };

void init_ir(PROTOCOL protocol);

void send_ready(PROTOCOL protocol);

void send_start(PROTOCOL protocol);

void send_stop(PROTOCOL protocol);

void send_menu_mode(PROTOCOL protocol);

void send_menu_up(PROTOCOL protocol);

void send_menu_down(PROTOCOL protocol);

#endif