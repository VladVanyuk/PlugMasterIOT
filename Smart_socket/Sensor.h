#ifndef SENSORsor_h
#define SENSORsor_h

#include <Arduino.h>
#include "pins.h"

#define CALIBRATION 355.55

int max_val;
int new_val;
int old_val = 0;
float rms;
float IRMS;

void SENSORsorSetup() {
  pinMode(SENSOR, INPUT);
}

void SENSORsorRead() {
  new_val = analogRead(SENSOR);
  if(new_val > old_val) {
    old_val = new_val;
  }
  
  else {
    delayMicroseconds(50);
    new_val = analogRead(SENSOR);
    if(new_val < old_val) {
      max_val = old_val;
      old_val = 0;
    }
    
    rms = max_val * 5.00 * 0.707 / 1024;
    
    IRMS = rms * CALIBRATION;
    
    Serial.print("  Current value in mA: ");
    Serial.println(IRMS);
  }
}


#endif //SENSORsor_h
