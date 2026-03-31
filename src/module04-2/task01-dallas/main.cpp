#include <OneWire.h>
#include <DallasTemperature.h>

#define ECL_SERIAL_SPEED 115200 // Enables Serial

#define ECL_WIFI_SSID "IOT15"         // Enables Wifi (needed for OTA, MQTT, or Telnet)
#define ECL_WIFI_PASSWORD "iotempire" // optional, default: "iotempire"

#define ECL_OTA_HOSTNAME "esp32-minikitA" // Enables OTA
#define ECL_OTA_PASSWORD "iotempower"     // optional, default: "iotempower"

#define ECL_TELNET_PORT 23 // Enables logs via Telnet

#define ECL_MQTT_SERVER "192.168.14.1"   // Enables MQTT
#define ECL_MQTT_CLIENT "esp32-minikitA" // optional, default: "node"
#define ECL_MQTT_PORT 1883               // optional, default: 1883

#include "ECL.h"

// Setup a oneWire instance
OneWire oneWire(21);

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

void setup()
{
    ECL::begin();    // initializes OTA, MQTT, Telnet
    sensors.begin(); // Start the DS18B20 sensor
    ECL::log.println("Setup complete");
}

void loop()
{
    ECL::loop(); // handles OTA, MQTT, Telnet Buttons etc

    sensors.requestTemperatures();            // Send the command to get temperatures
    float tempC = sensors.getTempCByIndex(0); // Read temperature in Celsius

    ECL::mqttPublish("temperature", String(tempC).c_str());
    ECL::log.printf("Temperature: %.1f C°\n", tempC); // printf via Serial & Telnet
    delay(1000);
}