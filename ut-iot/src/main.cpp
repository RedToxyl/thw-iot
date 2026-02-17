#include <Arduino.h>

const int pin = D0;   // GPIO 16

void setup() {
  pinMode(pin, OUTPUT);
}

void loop() {
  digitalWrite(pin, HIGH);  // Set pin to 3.3V
  delay(100);
  digitalWrite(pin, LOW);   // Set pin to 0V
  delay(100);
}
