#ifndef SIRC_H
#define SIRC_H

#include <Arduino.h>
#include <io.h>
#include <pinout.h>

void sirc_init();

void sirc_send_ready();

void sirc_send_start();

void sirc_send_stop();

#endif