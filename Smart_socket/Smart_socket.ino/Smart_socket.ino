#include <Arduino.h>
#include <Wire.h>
#include "ENCinter.h"
#include "OledDisplay.h"
#include "WIfi_client.h"

void setup(){
  Serial.begin(115200);

  Oled_setup();
  encoder_setup();
  wifi_setup();
}

void loop() {
  encoder_handler();
  led_oled();
  wifi_client();
}
