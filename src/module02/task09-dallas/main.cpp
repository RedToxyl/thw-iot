#include <OneWire.h>
#include <DallasTemperature.h>

// Setup a oneWire instance
OneWire oneWire(D2);

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  sensors.begin(); // Start the DS18B20 sensor
}

void loop() {
  Serial.print("Requesting temperature...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  float tempC = sensors.getTempCByIndex(0); // Read temperature in Celsius

  // Print the temperature in Celsius
  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.println("°C");

  delay(1000); // Wait 1 second before next reading
}
