#include <PubSubClient.h>
#include <ArduinoOTA.h>
#include <SoftwareSerial.h>
#include <U8g2lib.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 25
#define NUM_LEDS 10

const char *ssid = "IOT15";
const char *password = "iotempire";
const char *hostname = "esp32-minikitB";
const char *ota_password = "iotempower";

const char *mqtt_topic = "led-button";
const char *mqtt_server = "192.168.14.1";

WiFiClient espClient;
PubSubClient client(espClient);
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
U8G2_SSD1306_64X48_ER_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);


int mode = 2;

// ---------------- WIFI ----------------
void setup_wifi()
{
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    WiFi.setHostname(hostname);
    ArduinoOTA.setHostname(hostname);

    Serial.println("\nWiFi connected");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
}

// ---------------- SERIAL SETUP -----------------

// ---------------- MQTT CALLBACK ----------------

void callback(char *topic, byte *payload, unsigned int length)
{
    String message;
    for (unsigned int i = 0; i < length; i++)
    {
        message += (char)payload[i];
    }

    if (String(topic) == mqtt_topic)
    {
        mode = (mode + 1) % 3; // Cycle through modes
        Serial.println("Mode set to: " + String(mode));

        u8g2.clearBuffer();
        u8g2.drawStr(0, 10, ("Mode: " + String(mode)).c_str());
        u8g2.sendBuffer();        
    }
};

// ---------------- MQTT RECONNECT ----------------
void reconnect()
{
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");

        String clientId = hostname;
        clientId += String(random(0xffff), HEX);

        if (client.connect(clientId.c_str()))
        {
            Serial.println("connected");
            client.publish(hostname, "Hello World");
            client.subscribe(mqtt_topic);
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" retry in 5s");

            // IMPORTANT: keep OTA alive during wait
            for (int i = 0; i < 50; i++)
            {
                ArduinoOTA.handle();
                delay(100);
            }
        }
    }
}

// ---------------- OTA SETUP ----------------
void setup_ota()
{
    ArduinoOTA.setHostname(hostname);
    ArduinoOTA.setPassword(ota_password);

    ArduinoOTA.onStart([]()
                       { Serial.println("OTA Start"); });

    ArduinoOTA.onEnd([]()
                     { Serial.println("\nOTA End"); });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                          { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });

    ArduinoOTA.onError([](ota_error_t error)
                       { Serial.printf("Error[%u]\n", error); });

    ArduinoOTA.begin();
    Serial.println("OTA Ready");
}

// ---------------- SETUP ----------------
void setup()
{
    pinMode(BUILTIN_LED, OUTPUT);
    Serial.begin(115200);

    setup_wifi();
    setup_ota();

    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    pinMode(LED_PIN, OUTPUT); // LED pin

    // Initialize the display
    u8g2.begin();
    // Clear the display buffer
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_5x7_tr); // Choose a suitable font

    // initialize neoPixel strip
    strip.begin();
    strip.setPixelColor(0, strip.Color(255, 0, 0)); // Red
    strip.show();

}

// ---------------- LOOP ----------------
int current_led = 0;
void loop()
{
    ArduinoOTA.handle(); // MUST run frequently

    if (!client.connected())
        reconnect();

    client.loop();

    switch (mode){
        case 0:
            // Mode 0: All LEDs off
            for (int i = 0; i < NUM_LEDS; i++)
            {
                strip.setPixelColor(i, strip.Color(0, 0, 0)); // Off
            }
            strip.show();
            break;

        case 1:
            // Mode 1: All LEDs on (Purple)
            for (int i = 0; i < NUM_LEDS; i++)
            {
                strip.setPixelColor(i, strip.Color(122, 122, 0));
            }
            strip.show();
            break;

        case 2:
            // Mode 2: Cycling LEDs
            current_led = (current_led + 1) % NUM_LEDS; // Cycle through LEDs
            strip.setPixelColor(current_led, strip.Color(122, 0, 0)); // Red
            strip.setPixelColor((current_led + 29) % NUM_LEDS, strip.Color(0, 122, 0)); // Green
            strip.setPixelColor((current_led + 28) % NUM_LEDS, strip.Color(0, 0, 122)); // Blue
            strip.setPixelColor((current_led + 27) % NUM_LEDS, strip.Color(122, 122, 0)); // Yellow
            strip.setPixelColor((current_led + 26) % NUM_LEDS, strip.Color(122, 0, 122)); // Purple
            strip.setPixelColor((current_led + 25) % NUM_LEDS, strip.Color(0, 0, 0));
            strip.show();}
    delay(400); // Adjust speed of cycling
}