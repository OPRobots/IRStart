#include <control.h>

static bool started = false;

void module_start(void) {
  digitalWrite(PIN_SIGNAL, HIGH);
  digitalWrite(PIN_LED, HIGH);
  started = true;
}

void module_stop(void) {
  digitalWrite(PIN_SIGNAL, LOW);
  digitalWrite(PIN_LED, LOW);
  started = false;
}

bool module_is_started(void) {
  return started;
}

void module_feedback(void) {
  for (uint8_t i = 0; i < 5; i++) {
    digitalWrite(PIN_LED, HIGH);
    delay(80 - (15 * i));
    digitalWrite(PIN_LED, LOW);
    delay(80 - (15 * i));
  }
  delay(1500);
}