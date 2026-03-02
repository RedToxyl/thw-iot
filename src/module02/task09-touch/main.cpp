#include <Wire.h>
#include "Adafruit_MPR121.h"

// You can have up to 4 on one I2C bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();


void setup() {
  Serial.begin(115200);
  while (!Serial) { // Wait for serial port to connect (for Arduino Leonardo/Micro)
    delay(10);
  }

  Serial.println("Adafruit MPR121 Capacitive Touch sensor test");

  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");
}

void loop() {
  // Get the currently touched pads
  uint16_t touched = cap.touched();

  for (uint8_t i = 0; i < 2; i++) {
    // Check if this sensor is touched
    if (touched & (1 << i)) {
      Serial.print("Sensor ");
      Serial.print(i);
      Serial.println(" touched");
    }
  }
  // Put a delay so the serial output is not too overwhelming
  delay(100);
}
