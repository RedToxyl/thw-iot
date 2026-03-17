#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

// ===== WiFi =====
const char *ssid = "IOT15";
const char *password = "iotempire";

// ===== MQTT =====
const char *mqtt_server = "192.168.14.1";

WiFiClient espClient;
PubSubClient client(espClient);

// ===== OTA =====
const char* hostname = "ota-test";
const char* ota_password = "iotempower";

// ===== Misc =====
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 10;
bool flashing = false;
ulong startTime = 0;
const int ledPin = BUILTIN_LED;

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

// ---------------- MQTT CALLBACK ----------------

void callback(char* topic, byte* payload, unsigned int length) {
 String message;
 for (unsigned int i = 0; i < length; i++) {
   message += (char)payload[i];
 }


 if (String(topic) == "alarm") {
   if (message == "on") {
     flashing = true;
     startTime = millis();
   } else if (message == "off") {
     flashing = false;
     analogWrite(ledPin, 0); // Turn off LED
   }
 }
}

// ---------------- MQTT RECONNECT ----------------
void reconnect()
{
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");

        String clientId = "ESP8266Client-";
        clientId += String(random(0xffff), HEX);

        if (client.connect(clientId.c_str()))
        {
            Serial.println("connected");
            client.publish("m1_mini", "hello world");
            client.subscribe("alarm");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" retry in 5s");

            // IMPORTANT: keep OTA alive during wait
            for (int i = 0; i < 50; i++) {
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

    ArduinoOTA.onStart([]() {
        Serial.println("OTA Start");
    });

    ArduinoOTA.onEnd([]() {
        Serial.println("\nOTA End");
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });

    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]\n", error);
    });

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
    startTime = millis();
}

// ---------------- LOOP ----------------

void add_to_loop() {
 if (flashing) {
   unsigned long currentTime = millis();
   if (currentTime - startTime < 6000) { // 6 seconds
     int brightness = (sin((currentTime - startTime) / 1000.0 * PI) * 127.5) + 127.5;
     analogWrite(ledPin, brightness);
   } else {
     flashing = false;
     analogWrite(ledPin, 0); // Turn off LED
   }
 }
}


void loop()
{
    ArduinoOTA.handle();   // MUST run frequently

    if (!client.connected())
        reconnect();

    client.loop();
    add_to_loop();
}