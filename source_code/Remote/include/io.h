#ifndef IO_H
#define IO_H

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <pinout.h>


void set_rgb(int led, int r, int g, int b);
void set_led(bool state);
void blink_led(int ms);
void error_led();

bool get_btn_start();
bool get_btn_stop();
bool get_btn_ready();
bool get_btn_mode();
unsigned char get_dip_switch();

#endif