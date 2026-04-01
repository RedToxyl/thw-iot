#include <ArduinoOTA.h>
#include <SoftwareSerial.h>


const char *ssid = "IOT15";
const char *password = "iotempire";
const char *ota_password = "iotempower";
const char* hostname = "esp32-minikitB";

SoftwareSerial mySerial(33, 35);  // RX, TX
WiFiClient espClient;
U8G2_SSD1306_64X48_ER_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

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

    // Initialize the display
    u8g2.begin();
    // Clear the display buffer
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_5x7_tr); // Choose a suitable font
}

// ---------------- LOOP ----------------
String input = "";
void loop()
{
    ArduinoOTA.handle();   // MUST run frequently

    while (mySerial.available() > 0 and input[input.length() - 1] != '\n') {
        char c = mySerial.read();
        input += c;
    }
    if (input.length() > 0 and input[input.length() - 1] == '\n') {
        u8g2.clearBuffer();
        Serial.print("Received from UART1: ");
        Serial.print(input);
        u8g2.drawStr(0, input.length()-1, input.c_str());
        u8g2.sendBuffer();
        input = "";
    }
}