#include <Arduino.h>
#include <EncButton.h>

#define ENC_PIN_R D7
#define ENC_PIN_L D6
#define ENC_PIN_BTN D5

#define SERIAL_SPD 115200

EncButton eb(ENC_PIN_R, ENC_PIN_L, ENC_PIN_BTN);

int britnes;
bool state;

void setup() 
{
    Serial.begin(SERIAL_SPD);
    britnes = 0;
    state = 1;
    pinMode(LED_BUILTIN, OUTPUT);
   // digitalWrite(LED_BUILTIN, state);
   analogWrite(LED_BUILTIN, britnes = 127);



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

void change_state()
{
  state = !state;
  digitalWrite(LED_BUILTIN, state);
}

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

void loop() 
{
    eb.tick();


    // обработка поворота раздельная
    if (eb.left()) pwm_led_dwn();
    if (eb.right()) pwm_led_up();
    if (eb.leftH()) Serial.println("leftH");
    if (eb.rightH()) Serial.println("rightH");


    if(eb.press())
    {
      change_state();
    }

    if (eb.click()) Serial.println("click");

    if (eb.release()) {
      Serial.println("release");

      Serial.print("clicks: ");
      Serial.print(eb.getClicks());
      Serial.print(", steps: ");
      Serial.print(eb.getSteps());
      Serial.print(", press for: ");
      Serial.print(eb.pressFor());
      Serial.print(", hold for: ");
      Serial.print(eb.holdFor());
      Serial.print(", step for: ");
      Serial.println(eb.stepFor());
    }

   
   

  

   
}