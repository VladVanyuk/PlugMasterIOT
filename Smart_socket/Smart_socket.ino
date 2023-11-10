#include <Arduino.h>
#include <EncButton.h>

#define ENC_PIN_R D7
#define ENC_PIN_L D6
#define ENC_PIN_BTN D5

#define SERIAL_SPD 115200

#ifndef ON
#define ON 0
#endif

#ifndef OFF
#define OFF 1
#endif

EncButton eb(ENC_PIN_R, ENC_PIN_L, ENC_PIN_BTN);

bool state;


void encoder_setup()
{
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

void setup() 
{
    Serial.begin(SERIAL_SPD);

    state = OFF;
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, state);

    encoder_setup();
}

void change_state()
{
  state = !state;
  digitalWrite(LED_BUILTIN, state);
}

/*
void pwm_led_up()
{
  if(state == 0){
  britnes++;
  analogWrite(LED_BUILTIN, britnes);
  }
}

void pwm_led_dwn()
{
  if(state == 0){
  britnes--;
  analogWrite(LED_BUILTIN, britnes);
  }
}
*/

void encoder_handler()
{
  if (eb.press())
  {
    change_state();
  }

  if (eb.click()) Serial.println("click");
}


void loop() 
{
  eb.tick();
  encoder_handler();
}