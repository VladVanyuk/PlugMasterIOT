#include <Arduino.h>
#include <Wire.h>
#include <EncButton.h>

#define ENC_PIN_R D7
#define ENC_PIN_L D6
#define ENC_PIN_BTN D5

#define ON 0
#define OFF 1

EncButton eb(ENC_PIN_R, ENC_PIN_L, ENC_PIN_BTN);

bool state = OFF;

void encoder_setup() 
{

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, state);
 eb.setBtnLevel(LOW);
  eb.setClickTimeout(500);
  eb.setDebTimeout(50);
  eb.setHoldTimeout(600);
  eb.setStepTimeout(200);

  eb.setEncReverse(0);
  eb.setEncType(EB_STEP4_LOW);
  eb.setFastTimeout(30);
  eb.counter = 0;
}



void change_state() {
  state = !state;
  digitalWrite(LED_BUILTIN, state);
}

void encoder_handler() {
  eb.tick();
  if (eb.press()) {
    change_state();
  }

  if (eb.click()) {
    Serial.println("click");
  }
}