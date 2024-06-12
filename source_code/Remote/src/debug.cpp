#include <debug.h>

void debug_all() {
  digitalWrite(LED, HIGH);
  ledcWrite(IR_CMD_PWM, 1024 * 1 / 3);
  delay(125);
  digitalWrite(LED, LOW);
  ledcWrite(IR_CMD_PWM, 0);

  Serial.print("BTN_START: ");
  Serial.print(!digitalRead(BTN_START));
  Serial.print(" | ");
  Serial.print("BTN_STOP: ");
  Serial.print(!digitalRead(BTN_STOP));
  Serial.print(" | ");
  Serial.print("BTN_READY: ");
  Serial.print(!digitalRead(BTN_READY));
  Serial.print(" | ");
  Serial.print("DIP_SW0: ");
  Serial.print(!digitalRead(DIP_SW_0));
  Serial.print(" | ");
  Serial.print("DIP_SW1: ");
  Serial.print(!digitalRead(DIP_SW_1));
  Serial.print(" | ");
  Serial.print("DIP_SW2: ");
  Serial.print(!digitalRead(DIP_SW_2));
  Serial.print(" | ");
  Serial.print("DIP_SW3: ");
  Serial.print(!digitalRead(DIP_SW_3));
  Serial.println();

  delay(125);
}