#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

bool previousLedState = OFF;
bool screenNeedsUpdate = false;

const long interval = 2000;
unsigned long previousMillis = 0;

void Oled_setup()
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
}

void oled_handler()
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;

    bool currentState = digitalRead(LED_BUILTIN);

    if (currentState != previousLedState)
    {
      screenNeedsUpdate = true;
      previousLedState = currentState;
    }

    if (screenNeedsUpdate)
    {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);

      if (currentState == ON)
      {
        display.println("LED is ON!");
      }
      else
      {
        display.println("LED is OFF!");
      }

      display.display();

      screenNeedsUpdate = false;
    }
  }
}
