#ifndef Sensor_h
#define Sensor_h
#include <Arduino.h>
#include "pins.h"

#define calibration_const 355.55

int max_val;
int new_val;
int old_val = 0;
float rms;
float IRMS;

void Sen_setup() {
  pinMode(Sen,INPUT);
}

void Sensor() {
  new_val = analogRead(Sen);
  if(new_val > old_val) {
    old_val = new_val;
  }
  
  else {
    delayMicroseconds(50);
    new_val = analogRead(Sen);
    if(new_val < old_val) {
      max_val = old_val;
      old_val = 0;
    }
    
    rms = max_val * 5.00 * 0.707 / 1024;
    IRMS = rms * calibration_const;
    
    Serial.print("  Current value in mA: ");
    Serial.println(IRMS);
  }
}


#endif //Sensor_h
