#include <Arduino.h>
#include "TimerOne.h"

#define MAX_INPUT 12
#define AINPUT A0
#define LED_PIN 10
#define REPORT_INTERVAL 500

char inputBuffer[MAX_INPUT + 1]; // Handles up to 90 bytes in a c-style string, with a null character termination.

void setup()
{
  Serial.begin(115200);  // initialization
  inputBuffer[0] = '\0'; //Initialize string to emtpy.

  Timer1.initialize(255);
  Timer1.pwm(LED_PIN, 0);

  // pinMode(AINPUT, INPUT);

  Serial.println("Begin:");

  // Setting 	Divisor 	Frequency
  // 0x01 	 	1  		31372.55
  // 0x02 	 	8 	 	3921.16
  // 0x03  		32  		980.39
  // 0x04 	 	64 	 	490.20   <--DEFAULT
  // 0x05 	 	128  		245.10
  // 0x06  		256  		122.55
  // 0x07 	 	1024  		30.64

  // TCCR2B = (TCCR2B & 0b11111000) | <setting>;
  // TCCR2B = (TCCR2B & 0b11111000) | 0x02; // 3.9kHz frequency

  // All frequencies are in Hz and assume a 16000000 Hz system clock.
}

void loop()
{
  static uint8_t newPWM;
  static uint16_t pwmVal;
  static uint16_t a_in;
  static uint16_t lastRead;

  a_in = analogRead(AINPUT);
  Timer1.setPwmDuty(LED_PIN, a_in);

  if (millis() - lastRead > REPORT_INTERVAL) {
    lastRead = millis();
    Serial.println(a_in);
  }

  // if (newPWM) {
  //   // analogWrite(LED_PIN, pwmVal);
  //   // TCCR2B = (TCCR2B & 0b11111000) | 0x02; // 3.9kHz frequency
  //   Timer1.setPwmDuty(LED_PIN, pwmVal);
  //   newPWM = 0x00;
  // }

  if (Serial.available() > 0)
  {
    char input = Serial.read();
    static int s_len; // static variables default to 0
    if (s_len >= MAX_INPUT)
    {
      // Have received already the maximum number of characters
      // Ignore all new input until line termination occurs
    }
    else if (input != '\n' && input != '\r')
    {
      inputBuffer[s_len++] = input;
    }
    else
    {
      // Have received a LF or CR character

      // INSERT YOUR CODE HERE TO PROCESS THE RECEIVED DATA //
      // YOU COULD COPY TO A NEW VARIABLE WITH strncpy() OR //
      // SET A FLAG TO SAY TO START SOME OTHER TASK         //
      Serial.print("RECEIVED MSG: ");
      Serial.println(inputBuffer);
      newPWM = 0x01;
      pwmVal = atoi(inputBuffer);

      memset(inputBuffer, 0, sizeof(inputBuffer));
      s_len = 0; // Reset input buffer here if you
                 // have already copied the data.
                 // If you don't reset here, then
                 // you can't start receiving more
                 // serial port data. This is your
                 // 'software' serial buffer, contrast
                 // with the hardware serial buffer.
    }
  }
}