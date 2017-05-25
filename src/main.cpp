
#include <Arduino.h>

int frame = 0;
void setup()
{
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(13, 0x1);

  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

void loop()
{
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(300);
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(300);

    Serial.println(frame);
    ++frame;
}
