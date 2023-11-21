#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSerif9pt7b.h>
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

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
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
void setup() {
  Serial.begin(115200);
 

    state = OFF;
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, state);

    encoder_setup();
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println("SSD1306 allocation failed");
    for(;;);
  }
  
}
void change_state()
{
  state = !state;
  digitalWrite(LED_BUILTIN, state);
}

void encoder_handler()
{
  if (eb.press())
  {
    change_state();
  }

  if (eb.click()) Serial.println("click");
}

void led_oled ()
{
  if (state == OFF) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println("LED is OFF!");
    display.display();
    
    display.clearDisplay();
    display.display();
  } else {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println("LED is ON!");
    display.display();
    
    display.clearDisplay();
    display.display();
  }

}


void loop() {
   eb.tick();
  encoder_handler();
  led_oled ();
}