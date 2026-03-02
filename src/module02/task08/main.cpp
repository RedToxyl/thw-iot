#include <Arduino.h>

const int pin_led = D6;   // GPIO 16

int state_led = LOW; // Initial state for LED

void setup() {
	Serial.begin(115200); // Start serial communication for debugging

	pinMode(pin_led, OUTPUT);
}

void loop() {
    for (int i = 0; i < 256; i++) {
        analogWrite(pin_led, i); // Gradually increase brightness
        delay(20);
    }
    for (int i = 255; i >= 0; i--) {
        analogWrite(pin_led, i); // Gradually decrease brightness
        delay(20);
    }
}
