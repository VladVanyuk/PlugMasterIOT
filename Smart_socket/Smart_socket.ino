#include <Arduino.h>
#include <Wire.h>

#include "pins.h"
#include "conf.h"

#include "Relay.h"
#include "Encoder.h"
#include "OledDisplay.h"
#include "WIfi_client.h"

#define SERIAL_SPD 115200

unsigned long i;
uint32_t s;

void serial_setup()
{
  Serial.begin(SERIAL_SPD);
  delay(500);
  Serial.flush();
}

void setup(){
  serial_setup();

#if (RELAY_SERVICE == 1)
  relay_setup();
#endif

#if (ENCODER_SERVICE == 1)
  encoder_setup();
#endif

#if (OLED_SERVICE == 1)
  Oled_setup();
#endif

#if (WIFI_SERVICE == 1)
  wifi_setup();
#endif
}

void loop() 
{
#if (RELAY_SERVICE == 1)
  relay_handler();
#endif

#if (OLED_SERVICE == 1)
  oled_handler();
#endif

#if (WIFI_SERVICE == 1)
  wifi_client();
#endif
}
