#include <WiFi.h>
#include <ArduinoOTA.h>

const char *ssid = "IOT15";
const char *password = "iotempire";
const char* hostname = "esp32-minikitA";


WiFiClient espClient;

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 10;

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


// ---------------- OTA SETUP ----------------
void setup_ota()
{
    ArduinoOTA.setHostname(hostname);
    ArduinoOTA.setPassword(password);

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
}

// ---------------- LOOP ----------------
void loop()
{
    ArduinoOTA.handle();   // MUST run frequently

    if (!client.connected())
        reconnect();

    client.loop();
}