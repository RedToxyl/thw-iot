#define ECL_MQTT_SERVER "192.168.14.1"
#define AC_PIN 22

#include "ECL.h"

bool status = false;
unsigned long lastSwitchTime = 0;
bool isPinHigh = false;

void mqttCallback(char *topic, char *payload)
{
    String mode = String(payload);
    if (!mode.equals("on") && !mode.equals("off"))
        return;
    ECL::log.printf("Received on [%s]: %s\n", topic, payload);
    status = mode.equals("on");
    if (status)
    {
        digitalWrite(AC_PIN, HIGH);
        isPinHigh = true;
        lastSwitchTime = millis();
    }
    else
    {
        digitalWrite(AC_PIN, LOW);
        isPinHigh = false;
    }
}

void report()
{
    String mode = status ? "on" : "off";
    ECL::log.printf("Status: %s\n", mode.c_str());
    ECL::mqttPublish("ac/living_room/status", mode.c_str());
}

void setup()
{
    pinMode(AC_PIN, OUTPUT);
    ECL::begin();
    ECL::mqttSubscribe("ac/living_room", mqttCallback);
    ECL::setInterval(5000, report); // FIXME IT BREAKS EVERETHING!!!
}

void loop()
{
    ECL::loop();
    unsigned long t = millis();
    if (status)
    {
        if (isPinHigh)
        {
            if (t - lastSwitchTime >= 500)
            {
                digitalWrite(AC_PIN, LOW);
                isPinHigh = false;
                lastSwitchTime = t;
            }
        }
        else
        {
            if (t - lastSwitchTime >= 1500)
            {
                digitalWrite(AC_PIN, HIGH);
                isPinHigh = true;
                lastSwitchTime = t;
            }
        }
    }
}