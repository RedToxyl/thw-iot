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

int mode = 0;

int min_temp = -10;
int max_temp = 20;
float current_temp = min_temp;
float step = 0.5;

bool ac_on = false;

void mqttCallback(char *topic, char *payload)
{
    ECL::log.printf("Received on [%s]: %s", topic, payload); // printf via Serial & Telnet
    if (strcmp(topic, "simulation") == 0) {
        if (strcmp(payload, "start") == 0) {
        mode = 1;
        current_temp = min_temp;
    }
    else if (strcmp(payload, "stop") == 0) {
        mode = 0;
        current_temp = min_temp;
    }
    }
    if (strcmp(topic, "ac") == 0) {
        ac_on = strcmp(payload, "on") == 0;
    }
}

void setup()
{
    ECL::begin(); // initializes OTA, MQTT, Telnet
    ECL::mqttSubscribe("simulation", mqttCallback);
    ECL::mqttSubscribe("ac", mqttCallback);
    ECL::log.println("Setup complete");
}

void loop()
{
    ECL::loop(); // handles OTA, MQTT, Telnet Buttons etc
    if (mode == 0) {
        delay(1000);
        return;
    }

    switch (mode)
    {
    case 1:
        current_temp += step;
        if (current_temp >= max_temp)
            mode = 2;
        break;
    case 2:
        current_temp -= step;
        if (current_temp <= min_temp)
            mode = 1;
        break;
    }
    ECL::mqttPublish("temperature", String(current_temp).c_str());
    ECL::log.printf("AC is %s\n", ac_on ? "ON" : "OFF");
    delay(1000);
}