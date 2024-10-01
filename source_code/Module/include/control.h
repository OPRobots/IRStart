#ifndef CONTROL_H
#define CONTROL_H

#include "pinout.h"
#include <Arduino.h>

void module_start(void);
void module_stop(void);
bool module_is_started(void);
void module_feedback(void);

#endif