#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EncButton.h>

#define ENC_PIN_R D7
#define ENC_PIN_L D6
#define ENC_PIN_BTN D5

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define ON 0
#define OFF 1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
EncButton eb(ENC_PIN_R, ENC_PIN_L, ENC_PIN_BTN);

bool previousLedState = OFF;
bool screenNeedsUpdate = false;
bool state = OFF;

const long interval = 2000;
unsigned long previousMillis = 0;

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

void setup() {
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, state);

  encoder_setup();
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

void led_oled() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Отримати стан світлодіода
    bool currentState = digitalRead(LED_BUILTIN);

    // Перевірити зміну стану світлодіода
    if (currentState != previousLedState) {
      screenNeedsUpdate = true;
      previousLedState = currentState;
    }

    if (screenNeedsUpdate) {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);

      if (currentState == ON) {
        display.println("LED is ON!");
      } else {
        display.println("LED is OFF!");
      }

      display.display();

      screenNeedsUpdate = false;
    }
  }
}

void loop() {
  encoder_handler();
  led_oled();
}
