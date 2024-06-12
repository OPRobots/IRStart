#ifndef NEC_H
#define NEC_H

#include <Arduino.h>
#include <io.h>
#include <pinout.h>

void nec_init();

void nec_send_ready();

void nec_send_start();

void nec_send_stop();

#endif