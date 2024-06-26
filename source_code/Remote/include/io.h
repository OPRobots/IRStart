#ifndef IO_H
#define IO_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <pinout.h>

void set_rgb(int led, int r, int g, int b);
void set_led(bool state);
void blink_led(int ms);
void error_led();

bool get_btn_start();
bool get_btn_stop();
bool get_btn_ready();
unsigned char get_dip_switch();

#endif