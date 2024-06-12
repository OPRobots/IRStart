#include <config.h>

void init_components() {
  Serial.begin(115200);

  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

  pinMode(NEOPIXEL, OUTPUT);
  digitalWrite(NEOPIXEL, LOW);

  pinMode(IR_CMD, OUTPUT);
  digitalWrite(IR_CMD, LOW);

  pinMode(IR_PROG, OUTPUT);
  digitalWrite(IR_PROG, LOW);

  pinMode(BTN_START, INPUT);
  pinMode(BTN_STOP, INPUT);
  pinMode(BTN_READY, INPUT);

  pinMode(DIP_SW_0, INPUT);
  pinMode(DIP_SW_1, INPUT);
  pinMode(DIP_SW_2, INPUT);
  pinMode(DIP_SW_3, INPUT);
}