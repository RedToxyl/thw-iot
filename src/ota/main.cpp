#define ECL_SERIAL_SPEED 115200 // Enables Serial

#define ECL_WIFI_SSID "IOT15"         // Enables Wifi (needed for OTA, MQTT, or Telnet)
#define ECL_WIFI_PASSWORD "iotempire" // optional, default: "iotempire"

#define ECL_TELNET_PORT 23 // Enables logs via Telnet


#define ECL_OTA_HOSTNAME "esp32-minikitB" // Enables OTA
#define ECL_OTA_PASSWORD "iotempower"     // optional, default: "iotempower"

#include "ECL.h"

void setup()
{
    ECL::begin(); // initializes OTA, MQTT, Telnet
}

void loop()
{
    ECL::loop(); // handles OTA, MQTT, Telnet Buttons etc
}