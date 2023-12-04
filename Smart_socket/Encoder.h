
#ifndef Encoder_h
#define Encoder_h 

#include <Arduino.h>
#include <EncButton.h>

#include "pins.h"
#include "Relay.h"

#define ENC_CLICK_TIMEOUT 500
#define ENC_DEB_TIMEOUT 50
#define ENC_HOLD_TIMEOUT 600
#define ENC_STEP_TIMEOUT 200
#define ENC_FAST_TIMEOUT 30

EncButton EncBtn(ENC_PIN_R, ENC_PIN_L, ENC_PIN_BTN);

IRAM_ATTR void btn_isr()
{
  relay_state_changed = true;
  EncBtn.tickISR();
}

IRAM_ATTR void enc_isr()
{
  EncBtn.tickISR();
}

void encoder_callback()
{
  Serial.print("callback: ");
  switch (EncBtn.action())
  {
  case EB_PRESS:
    Serial.println("press");
    break;
  case EB_HOLD:
    Serial.println("hold");
    break;
  case EB_STEP:
    Serial.println("step");
    break;
  case EB_RELEASE:
    Serial.print("release. steps: ");
    Serial.print(EncBtn.getSteps());
    Serial.print(", press for: ");
    Serial.print(EncBtn.pressFor());
    Serial.print(", hold for: ");
    Serial.print(EncBtn.holdFor());
    Serial.print(", step for: ");
    Serial.println(EncBtn.stepFor());
    break;
  case EB_CLICK:
    Serial.println("click");
    break;
  case EB_CLICKS:
    Serial.print("clicks ");
    Serial.println(EncBtn.getClicks());
    break;
  case EB_TURN:
    Serial.print("turn ");
    Serial.print(EncBtn.dir());
    Serial.print(" ");
    Serial.print(EncBtn.fast());
    Serial.print(" ");
    Serial.println(EncBtn.pressing());
    break;
  case EB_REL_HOLD:
    Serial.println("release hold");
    break;
  case EB_REL_HOLD_C:
    Serial.print("release hold clicks ");
    Serial.println(EncBtn.getClicks());
    break;
  case EB_REL_STEP:
    Serial.println("release step");
    break;
  case EB_REL_STEP_C:
    Serial.print("release step clicks ");
    Serial.println(EncBtn.getClicks());
    break;
  default:
    Serial.println();
  }
}

void encoder_setup()
{
  EncBtn.setBtnLevel(LOW);
  EncBtn.setClickTimeout(ENC_CLICK_TIMEOUT);
  EncBtn.setDebTimeout(ENC_DEB_TIMEOUT);
  EncBtn.setHoldTimeout(ENC_HOLD_TIMEOUT);
  EncBtn.setStepTimeout(ENC_STEP_TIMEOUT);
  EncBtn.setFastTimeout(ENC_FAST_TIMEOUT);
  EncBtn.setEncReverse(false);
  EncBtn.setEncType(EB_STEP4_LOW);
  EncBtn.counter = 0;

  attachInterrupt(digitalPinToInterrupt(ENC_PIN_BTN), btn_isr, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENC_PIN_L), enc_isr, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_PIN_R), enc_isr, CHANGE);
  EncBtn.setEncISR(true);
  EncBtn.attach(encoder_callback);
}

void encoder_handler()
{
   EncBtn.tick();
}

#endif //Encoder_h