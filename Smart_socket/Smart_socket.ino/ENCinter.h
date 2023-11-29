#include <Arduino.h>
#include <Wire.h>
#include <EncButton.h>

#define ENC_PIN_R D7
#define ENC_PIN_L D6
#define ENC_PIN_BTN D5

#define switched true // value if the button switch has been pressed
#define triggered true // controls interrupt handler

#define debounce 10 // time to wait in milliseconds

#define ON 0
#define OFF 1

EncButton eb(ENC_PIN_R, ENC_PIN_L, ENC_PIN_BTN);

bool state = OFF;

void function_on_interrupt()
{
   Serial.println("STOP");

  if (read_button() == switched) {
    state = HIGH - state;
    digitalWrite(LED_BUILTIN, state);
  } else {
    Serial.println("don't click");
  }
}

void encoder_setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(button_switch, INPUT_PULLUP);
  //pinMode l, r
  attachInterrupt(digitalPinToInterrupt(button_switch()ENC_PIN_BTN), function_on_interrupt, CHANGE);
  
  eb.setClickTimeout(500);
  eb.setDebTimeout(50);
  eb.setHoldTimeout(600);
  eb.setStepTimeout(200);

  eb.setEncReverse(0);
  eb.setEncType(EB_STEP4_LOW);
  eb.setFastTimeout(30);
  eb.counter = 0;
}

void encoder_handler() {
  
}