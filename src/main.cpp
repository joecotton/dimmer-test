#include <Arduino.h>
#include "TimerOne.h"
#include "FlexiTimer2.h"

#define MAX_INPUT 12
#define AINPUT A0
#define LED_PIN 10
#define REPORT_INTERVAL 100

#define DIMMER_MIN 0
#define DIMMER_MAX_DEFAULT 100
#define DIMMER_DELAY_MS 5

#define DITHER_UNITS 1 // 1/1000ms

#define FLEXI_LOW (uint8_t)(((pwmVal & 0x003F) << 2))
#define FLEXI_HIGH ((pwmVal >> 6))

uint16_t dimmer_max = DIMMER_MAX_DEFAULT;

uint16_t pwmVal = 0x0000; // use top 10 bits for Timer1, Lower 6 for Dither
//   F E D C B A 9 8 7 6 5 4 3 2 1 0
// b 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
// |--------------------|-----------|
//       Timer1 PWM     FlexiTimer Duty
// Timer1PWM = (pwmVal >> 6)
// dither_duty = (((pwmVal && 0x3F) << 0))

// uint8_t dither_flag = 0;
// uint8_t dither_cycletime = 0xFF;
uint8_t dither_cycle = 0x00;
uint8_t dither_duty = 0x00;

// char inputBuffer[MAX_INPUT + 1]; // Handles up to 90 bytes in a c-style string, with a null character termination.

void temporaldither() {
  // dither_cycle++;
  dither_cycle += 4;
  // Ontime first, then offtime
  if (dither_cycle>FLEXI_LOW) {
    // OffTime
    Timer1.setPwmDuty(LED_PIN, FLEXI_HIGH);
    // digitalWrite(LED_BUILTIN, LOW);
  }
  else
  {
    // OnTime
    Timer1.setPwmDuty(LED_PIN, FLEXI_HIGH + 1);
    // digitalWrite(LED_BUILTIN, HIGH);
  }
}

void setup()
{
  Serial.begin(115200);  // initialization
  // inputBuffer[0] = '\0'; //Initialize string to emtpy.

  pinMode(LED_BUILTIN, OUTPUT);
  Timer1.initialize(255);
  Timer1.pwm(LED_PIN, 0);

  FlexiTimer2::set(DITHER_UNITS, 1.0 / 10000, temporaldither);
  FlexiTimer2::start();

  // pinMode(AINPUT, INPUT);

  Serial.println("Begin:");
}

void loop()
{
  static uint32_t lastRead;
  static uint32_t lastDim;
  static uint8_t dir;

  dimmer_max = analogRead(AINPUT);

  if (millis() - lastRead > REPORT_INTERVAL)
  {
    lastRead = millis();
    Serial.print("Full pwmVal:");
    Serial.println(pwmVal, HEX);
    Serial.print("Timer1PWM:");
    Serial.print(FLEXI_HIGH, HEX);
    Serial.print("/");
    Serial.println(dimmer_max, HEX);
    Serial.print("FlexiTimer Duty:");
    Serial.println(pwmVal & 0x003F, HEX);
    // Serial.println((uint8_t)FLEXI_LOW, HEX);
    Serial.print("dir:");
    Serial.println(dir);
    // Serial.print("millis():");
    // Serial.println(millis());
  }

  if (millis() - lastDim > DIMMER_DELAY_MS)
  {
    lastDim = millis();
    if (FLEXI_HIGH <= DIMMER_MIN)
    {
      dir = 1;
    }
    else if (FLEXI_HIGH >= dimmer_max)
    {
      dir = 0;
    }
    if (dir) {
      pwmVal++;
    } else {
      pwmVal--;
    }
    // dither_duty = FLEXI_LOW;
    // Timer1.setPwmDuty(LED_PIN, FLEXI_HIGH);
  }
 }