#include <Arduino.h>
#include <Wire.h>
#include <EncButton.h>

#define ENC_PIN_R D7
#define ENC_PIN_L D6
#define ENC_PIN_BTN D5
#define button_switch ENC_PIN_BTN // Оголошуємо button_switch як ENC_PIN_BTN

#define switched true // value if the button switch has been pressed
#define triggered true // controls interrupt handler
#define interrupt_trigger_type RISING // interrupt triggered on a RISING input
#define debounce 10 // time to wait in milliseconds

#define ON 0
#define OFF 1

EncButton eb(ENC_PIN_R, ENC_PIN_L, ENC_PIN_BTN);

bool state = OFF;

volatile bool interrupt_process_status = {
  !triggered // start with no switch press pending, ie false (!triggered)
};
bool initialisation_complete = false; // inhibit any interrupts until initialization is complete

void button_interrupt_handler() {
  if (initialisation_complete == true) {
    if (interrupt_process_status == !triggered) {
      if (digitalRead(button_switch) == HIGH) {
        interrupt_process_status = triggered;
      }
    }
  }
}

bool read_button() {
  int button_reading;
  static bool switching_pending = false;
  static long int elapse_timer;
  if (interrupt_process_status == triggered) {
    button_reading = digitalRead(button_switch);
    if (button_reading == HIGH)  {
      switching_pending = true;
      elapse_timer  = millis();
    }
    if (switching_pending && button_reading == LOW) {
      if (millis() - elapse_timer >= debounce) {
        switching_pending = false;
        interrupt_process_status = !triggered;
        return switched;
      }
    }
  }
  return !switched;
}

void encoder_setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(button_switch, INPUT);
  attachInterrupt(digitalPinToInterrupt(button_switch), button_interrupt_handler, interrupt_trigger_type);
  initialisation_complete = true;
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
  if (read_button() == switched) {
    state = HIGH - state;
    digitalWrite(LED_BUILTIN, state);
  } else {
    Serial.println("don't click");
  }
}
