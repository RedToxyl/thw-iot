#include <Wire.h>
#include "Adafruit_MPR121.h"

// You can have up to 4 on one I2C bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

void setup()
{
    Serial.begin(115200);
    while (!Serial){
        delay(10);
    }
    delay(100);

    Serial.println("Adafruit MPR121 Capacitive Touch sensor test");

    if (!cap.begin(0x5A)){
        Serial.println("MPR121 not found, check wiring?");
        while (1);
    }
    Serial.println("MPR121 found!");

    cap.setThresholds(8, 4);
}

void loop()
{
    // Get the currently touched pads
    uint16_t touched = cap.touched();

    Serial.printf("Filtered data for pad 0: %d\n", cap.filteredData(0));
    Serial.printf("Baseline data for pad 0: %d\n", cap.baselineData(0));

    if (touched) Serial.println("Wet at time: " + String(millis()));
    Serial.println("");
    // Put a delay so the serial output is not too overwhelming
    delay(500);
}
