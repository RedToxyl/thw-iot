#include <PubSubClient.h>
#include <ArduinoOTA.h>
#include <SoftwareSerial.h>

const char *ssid = "IOT15";
const char *password = "iotempire";
const char* hostname = "esp32-minikitA";
const char* ota_password = "iotempower";

const char* mqtt_topic = "status";
const char *mqtt_server = "192.168.14.1";

WiFiClient espClient;
PubSubClient client(espClient);
SoftwareSerial mySerial(16, 17);  // RX, TX

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


// ---------------- MQTT CALLBACK ----------------



void callback(char* topic, byte* payload, unsigned int length) {
 String message;
 for (unsigned int i = 0; i < length; i++) {
   message += (char)payload[i];
 }


 if (String(topic) == mqtt_topic) {
   Serial.println("Status received: " + message);
   mySerial.println("Status received: " + message);
 }
}

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

    mySerial.begin(9600);  // UART1

    setup_wifi();
    setup_ota();

    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

// ---------------- LOOP ----------------
void loop()
{
    ArduinoOTA.handle();   // MUST run frequently

    if (!client.connected())
        reconnect();

    client.loop();
}