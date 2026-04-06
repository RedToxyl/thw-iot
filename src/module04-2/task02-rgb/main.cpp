#include <ECL.h>
#include <Adafruit_NeoPixel.h>

#define LED_NUM 7
#define BRIGHTNESS 0.2

int mode = 0; // 0 - off; 1 - awaiting rfid; 2 - rejection
int last_sonar = 0;

char* rfid_uid = "934766a";

Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_NUM, 16, NEO_GRB + NEO_KHZ800);

void acceptance_sound() {
    ECL::mqttPublish("buzzer", "1");
    delay(10);
    ECL::mqttPublish("buzzer", "2");
    delay(10);
    ECL::mqttPublish("buzzer", "3");
    delay(10);
    ECL::mqttPublish("buzzer", "4");
    delay(10);
    ECL::mqttPublish("buzzer", "5");
    delay(10);
    ECL::mqttPublish("buzzer", "6");
}

void denial_sound() {
    ECL::mqttPublish("buzzer", "6");
    delay(10);
    ECL::mqttPublish("buzzer", "5");
    delay(10);
    ECL::mqttPublish("buzzer", "4");
    delay(10);
    ECL::mqttPublish("buzzer", "3");
    delay(10);
    ECL::mqttPublish("buzzer", "2");
    delay(10);
    ECL::mqttPublish("buzzer", "1");
}

void led_set(uint8_t R, uint8_t G, uint8_t B)
{
    for (int i = 0; i < LED_NUM; i++)
    {
        leds.setPixelColor(i, leds.Color(R * BRIGHTNESS, G * BRIGHTNESS, B * BRIGHTNESS));
        delay(25);
        leds.show();
    }
}

void adjust_lights(const char* payload) {
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
    else if (strcmp(payload, "cyan") == 0)
    {
        led_set(0, 255, 255);
    }
    else if (strcmp(payload, "purple") == 0)
    {
        led_set(255, 0, 255);
    }
    else if (strcmp(payload, "orange") == 0)
    {
        led_set(255, 122, 0);
    }
    else if (strcmp(payload, "off") == 0)
    {
        led_set(0, 0, 0);
    }
}

void mqttCallback(const char *topic, const char *payload)
{
    if (strcmp(topic, "rgb") == 0) {adjust_lights(payload);}
        else if (strcmp(topic, "sonar") == 0 && atoi(payload) < 20) {
            last_sonar = millis();
            mode = 1;
            ECL::mqttPublish("display", "Present RFID");
            adjust_lights("orange");
        }
        else if (strcmp(topic, "rfid") == 0) {
            if (mode == 0) {
                mode = 2;
                ECL::mqttPublish("display", "Come Closer!");
                adjust_lights("red");
                delay(400);
                adjust_lights("off");
            }
            else if (mode == 1) {
                mode = 2;
                if (strcmp(payload, rfid_uid) == 0) {
                    ECL::mqttPublish("display", "Welcome Home");
                    adjust_lights("green");
                    delay(400);
                    acceptance_sound();
                }
                else {
                    ECL::mqttPublish("display", "Access Denied");
                    adjust_lights("red");
                    delay(400);
                    denial_sound();
                }
            }
        }
}

void setup()
{
    ECL::begin();
    ECL::mqttSubscribe("rgb", mqttCallback);
    ECL::mqttSubscribe("sonar", mqttCallback);
    ECL::mqttSubscribe("rfid", mqttCallback);
    leds.begin();
}

void loop()
{
    ECL::loop();
    if (mode == 1 && millis() - last_sonar > 5000) {
        mode = 0;
        ECL::mqttPublish("display", "Goodbye!");
        adjust_lights("off");
    }
    delay(100);
}