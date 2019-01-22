#include <Arduino.h>
#include "TimerOne.h"

#define MAX_INPUT 12
#define AINPUT A0
#define LED_PIN 10
#define REPORT_INTERVAL 500

#define DIMMER_MIN 0
#define DIMMER_MAX_DEFAULT 100
#define DIMMER_DELAY_MS 100

uint16_t dimmer_max = DIMMER_MAX_DEFAULT;

// char inputBuffer[MAX_INPUT + 1]; // Handles up to 90 bytes in a c-style string, with a null character termination.

void setup()
{
  Serial.begin(115200);  // initialization
  // inputBuffer[0] = '\0'; //Initialize string to emtpy.

  Timer1.initialize(255);
  Timer1.pwm(LED_PIN, 0);

  // pinMode(AINPUT, INPUT);

  Serial.println("Begin:");
}

void loop()
{
  static uint16_t pwmVal;
  static uint32_t lastRead;
  static uint32_t lastDim;
  static uint8_t dir;

  dimmer_max = analogRead(AINPUT);

  if (millis() - lastRead > REPORT_INTERVAL)
  {
    lastRead = millis();
    Serial.print("pwmVal:");
    Serial.print(pwmVal);
    Serial.print("/");
    Serial.println(dimmer_max);
    Serial.print("dir:");
    Serial.println(dir);
    Serial.print("millis():");
    Serial.println(millis());
  }

  if (millis() - lastDim > DIMMER_DELAY_MS)
  {
    lastDim = millis();
    if ( pwmVal==0) {
      dir = 1;
    } else if (pwmVal >= dimmer_max) {
      dir = 0;
    }
    if (dir) {
      pwmVal++;
    } else {
      pwmVal--;
    }
    Timer1.setPwmDuty(LED_PIN, pwmVal);
  }
 }