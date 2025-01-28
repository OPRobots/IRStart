#ifndef RC5_H
#define RC5_H

#include <Arduino.h>
#include <io.h>
#include <pinout.h>

void rc5_init();
void rc5_send_prog();
void rc5_send_start();
void rc5_send_stop();

void rc5_send_menu_mode();
void rc5_send_menu_up();
void rc5_send_menu_down();

#endif