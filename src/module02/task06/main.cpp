#include <Arduino.h>

const int pin_led = D6;   // GPIO 16
const int pin_button = D0;

int state_led = LOW; // Initial state for LED

void blink(int duration) {
	digitalWrite(pin_led, HIGH);
	delay(duration);
	digitalWrite(pin_led, LOW);
	delay(duration);
}

void setup() {
	Serial.begin(115200); // Start serial communication for debugging

	pinMode(pin_led, OUTPUT);
	pinMode(pin_button, INPUT);

	blink(50);
	blink(50);
	blink(100);
}

void loop() {
    int button_state = digitalRead(pin_button);
    
    if (button_state == HIGH) {
        state_led = !state_led; // Toggle LED state
        digitalWrite(pin_led, state_led);
        delay(200); // Debounce delay
        Serial.printf("Button pressed! LED turned %s\n", state_led ? "ON" : "OFF");
    }

    delay(20);
}
