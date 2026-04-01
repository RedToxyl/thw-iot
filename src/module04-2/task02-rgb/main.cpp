#include <ECL.h>
#include <Adafruit_NeoPixel.h>

#define LED_NUM 7
#define BRIGHTNESS 0.2

Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_NUM, 16, NEO_GRB + NEO_KHZ800);

void led_set(uint8_t R, uint8_t G, uint8_t B)
{
    for (int i = 0; i < LED_NUM; i++)
    {
        leds.setPixelColor(i, leds.Color(R * BRIGHTNESS, G * BRIGHTNESS, B * BRIGHTNESS));
        delay(25);
        leds.show();
    }
}

void mqttCallback(const char *topic, const char *payload)
{
    if (strcmp(payload, "red") == 0)
    {
        led_set(255, 0, 0);
    }
    else if (strcmp(payload, "green") == 0)
    {
        led_set(0, 255, 0);
    }
    else if (strcmp(payload, "blue") == 0)
    {
        led_set(0, 0, 255);
    }
    else if (strcmp(payload, "yellow") == 0)
    {
        led_set(0, 255, 255);
    }
    else if (strcmp(payload, "purple") == 0)
    {
        led_set(255, 0, 255);
    }
    else if (strcmp(payload, "orange") == 0)
    {
        led_set(255, 255, 0);
    }
    else if (strcmp(payload, "off") == 0)
    {
        led_set(0, 0, 0);
    }
}

void setup()
{
    ECL::begin();
    ECL::mqttSubscribe("rgb", mqttCallback);
    leds.begin();
}

void loop()
{
    ECL::loop();
    delay(100);
}