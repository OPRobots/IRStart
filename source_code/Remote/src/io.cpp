#include <io.h>

static Adafruit_NeoPixel pixels(3, NEOPIXEL, NEO_GRB + NEO_KHZ800);

static bool led_state = false;
static long last_blink = 0;

void set_rgb(int led, int r, int g, int b) {
  pixels.setPixelColor(led, pixels.Color(r, g, b));
  pixels.show();
}

void set_led(bool state) {
  digitalWrite(LED, !state);
  led_state = state;
}

void blink_led(int ms) {
  if (millis() - last_blink > ms) {
    set_led(!led_state);
    last_blink = millis();
  }
}

void error_led() {
  set_led(true);
  delay(50);
  set_led(false);
  delay(50);
  set_led(true);
  delay(50);
  set_led(false);
  delay(250);
}

bool get_btn_start() {
  bool state1 = digitalRead(BTN_START);
  delay(10);
  bool state2 = digitalRead(BTN_START);
  return state1 && state2;
}

bool get_btn_stop() {
  bool state1 = digitalRead(BTN_STOP);
  delay(10);
  bool state2 = digitalRead(BTN_STOP);
  return state1 && state2;
}

bool get_btn_ready() {
  bool state1 = digitalRead(BTN_READY);
  delay(10);
  bool state2 = digitalRead(BTN_READY);
  return state1 && state2;
}

unsigned char get_dip_switch() {
  unsigned char dip = 0;
  int DIP[4] = {DIP_SW_0, DIP_SW_1, DIP_SW_2, DIP_SW_3};
  for (int i = 0; i < 4; i++) {
    dip |= digitalRead(DIP[i]) << i;
  }
  return dip << 1;
}