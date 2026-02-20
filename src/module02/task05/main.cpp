#include <Arduino.h>

const int pin_green = D0;   // GPIO 16
const int pin_blue = D6;
const int pin_inbuilt = LED_BUILTIN; // GPIO 2

const unsigned long delay_green = 1200; // Delay for green LED in milliseconds
const unsigned long delay_blue = 500;   // Delay for blue LED in milliseconds

unsigned long state_green = 0; // Initial state for green LED
unsigned long state_blue = 0;  // Initial state for blue LED

unsigned long last_timestamp = 0;


void setup() {
	Serial.begin(115200); // Start serial communication for debugging
	pinMode(pin_green, OUTPUT);
	pinMode(pin_blue, OUTPUT);
	pinMode(pin_inbuilt, OUTPUT);

	digitalWrite(pin_green, LOW); // Ensure green LED starts off
	digitalWrite(pin_blue, LOW);  // Ensure blue LED starts off
	digitalWrite(pin_inbuilt, LOW); // Ensure inbuilt LED starts off

	last_timestamp = millis(); // Initialize the last timestamp
	Serial.println("Setup complete. Starting loop...");
}

void loop() {
	unsigned long delta_time = millis() - last_timestamp;
	last_timestamp = millis(); // Update the last timestamp

	state_green += delta_time;
	state_blue += delta_time;

	if (state_green >= delay_green) {
	digitalWrite(pin_green, HIGH);
	delay(50);
	digitalWrite(pin_green, LOW);

	state_green = 0; // Reset the state for green LED
	}

	if (state_blue >= delay_blue) {
	digitalWrite(pin_blue, HIGH);
	digitalWrite(pin_inbuilt, LOW);
	delay(100);
	digitalWrite(pin_blue, LOW);
	digitalWrite(pin_inbuilt, HIGH);

	state_blue = 0; // Reset the state for blue LEDs
	}

	delay(10);
}
