#include <Arduino.h>

#define MAX_INPUT 12
#define LED_PIN 11

char inputBuffer[MAX_INPUT + 1]; // Handles up to 90 bytes in a c-style string, with a null character termination.

void setup()
{
  Serial.begin(115200);  // initialization
  inputBuffer[0] = '\0'; //Initialize string to emtpy.
  Serial.println("Begin:");
  // pinMode(A0, OUTPUT);
}

void loop()
{
  static uint8_t newPWM;
  static uint16_t pwmVal;

  if (newPWM) {
    analogWrite(LED_PIN, pwmVal);
    newPWM = 0x00;
  }

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