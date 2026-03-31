#pragma once
#include <Arduino.h>
#include <vector>

// ========== VALIDATION & DEPENDENCIES ==========
// If OTA, MQTT, or Telnet are defined, we MUST have WiFi enabled.
#if defined(ECL_OTA_HOSTNAME) || defined(ECL_MQTT_SERVER) || defined(ECL_TELNET_PORT)
#ifndef ECL_WIFI_SSID
#error "ECL Error: ECL_WIFI_SSID must be defined to use OTA, MQTT, or Telnet!"
#endif
#endif

// ========== WIFI GLOBALS ==========
#if defined(ECL_WIFI_SSID)
#include <WiFi.h>
#ifndef ECL_WIFI_PASSWORD
#define ECL_WIFI_PASSWORD "iotempire"
#endif
WiFiClient wifiClient;
#endif

// ========== SOFTWARE SERIAL GLOBALS ==========
#if defined(ECL_SOFTWARE_SERIAL_RX) && defined(ECL_SOFTWARE_SERIAL_TX)
#include <SoftwareSerial.h>
#ifndef ECL_SOFTWARE_SERIAL_SPEED
#define ECL_SOFTWARE_SERIAL_SPEED 9600
#endif
SoftwareSerial softwareSerial(ECL_SOFTWARE_SERIAL_RX, ECL_SOFTWARE_SERIAL_TX);
#endif

// ========== TELNET GLOBALS ==========
#if defined(ECL_TELNET_PORT)
WiFiServer telnetServer(ECL_TELNET_PORT);
WiFiClient telnetClient;
#endif

// ========== MQTT GLOBALS ==========
#if defined(ECL_MQTT_SERVER)
#include <PubSubClient.h>
#ifndef ECL_MQTT_PORT
#define ECL_MQTT_PORT 1883
#endif
#ifndef ECL_MQTT_CLIENT
#define ECL_MQTT_CLIENT "node"
#endif
PubSubClient mqttClient(wifiClient);
struct MqttSubscription
{
    String topic;
    std::function<void(char *, byte *, unsigned int)> callback;
};
std::vector<MqttSubscription> _eclSubscriptions;
#endif

// ========== OTA GLOBALS ==========
#if defined(ECL_OTA_HOSTNAME)
#include <ArduinoOTA.h>
#ifndef ECL_OTA_PASSWORD
#define ECL_OTA_PASSWORD "iotempower"
#endif
#endif

// ========== GLOBALS ==========
std::vector<std::function<void()>> _eclLoopHandlers;

// ========== LIBRARY NAMESPACE ==========
namespace ECL
{
    inline void addToLoop(std::function<void()> cb)
    {
        _eclLoopHandlers.push_back(cb);
    }

    // --- Logging Functions ---
    inline void log(const String &msg)
    {
        Serial.println(msg);
#if defined(ECL_TELNET_PORT)
        if (telnetClient && telnetClient.connected())
            telnetClient.println(msg);
#endif
    }
    void logf(const char *format, ...)
    {
        char buffer[64];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);

        Serial.println(buffer);
#if defined(ECL_TELNET_PORT)
        if (telnetClient && telnetClient.connected())
            telnetClient.println(buffer);
#endif
    }
    // --- MQTT ---

#if defined(ECL_MQTT_SERVER)
    bool _mqttTopicMatch(const char *sub, const char *topic)
    {
        while (*sub && *topic)
        {
            if (*sub == '#')
                return true;
            if (*sub == '+')
            {
                while (*topic && *topic != '/')
                    topic++;
                sub++;
                if (*topic == '/' && *sub == '/')
                {
                    topic++;
                    sub++;
                }
                continue;
            }
            if (*sub != *topic)
                return false;
            sub++;
            topic++;
        }
        if (*sub == '#' && *(sub + 1) == '\0')
            return true;
        return (*sub == '\0' && *topic == '\0');
    }

    inline void _mqttRoute(char *topic, byte *payload, unsigned int length)
    {
        for (auto &sub : _eclSubscriptions)
            if (_mqttTopicMatch(sub.topic.c_str(), topic))
                sub.callback(topic, payload, length);
    }

    inline void mqttPublish(const char *topic, const char *payload)
    {
        mqttClient.publish(topic, payload);
    }
    inline void mqttSubscribe(const char *topic, std::function<void(char *, byte *, unsigned int)> cb)
    {
        _eclSubscriptions.push_back({topic, cb});
        if (mqttClient.connected())
            mqttClient.subscribe(topic);
    }

    inline void mqttSubscribe(const char *topic, std::function<void(char *, char *)> cb)
    {
        _eclSubscriptions.push_back(
            {topic, [cb](char *t, byte *p, unsigned int len)
             {
                 String payloadStr;
                 payloadStr.reserve(len + 1);
                 for (unsigned int i = 0; i < len; i++)
                     payloadStr += (char)p[i];
                 cb(t, (char *)payloadStr.c_str());
             }});
        if (mqttClient.connected())
            mqttClient.subscribe(topic);
    }
#endif

    // --- Initialization ---
    inline void begin()
    {
// Serial Setup
#if defined(ECL_SERIAL_SPEED)
        Serial.begin(ECL_SERIAL_SPEED);
#endif
// WiFi Setup
#if defined(ECL_WIFI_SSID)
        ECL::logf("Connecting to WiFi: %u", ECL_WIFI_SSID);
        WiFi.mode(WIFI_STA);
        WiFi.begin(ECL_WIFI_SSID, ECL_WIFI_PASSWORD);

#if defined(ECL_OTA_HOSTNAME)
        WiFi.setHostname(ECL_OTA_HOSTNAME);
#endif

        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print(".");
        }
        ECL::log("\nWiFi connected. IP: " + WiFi.localIP().toString());
#endif

// Telnet Setup
#if defined(ECL_TELNET_PORT)
        telnetServer.begin();
#endif

// Software Serial Setup
#if defined(ECL_SOFTWARE_SERIAL_RX) && defined(ECL_SOFTWARE_SERIAL_TX)
        softwareSerial.begin(ECL_SOFTWARE_SERIAL_SPEED);
#endif

// MQTT Setup
#if defined(ECL_MQTT_SERVER)
        mqttClient.setServer(ECL_MQTT_SERVER, ECL_MQTT_PORT);
        mqttClient.setCallback(_mqttRoute);
#endif
// OTA Setup
#if defined(ECL_OTA_HOSTNAME)
        ArduinoOTA.setHostname(ECL_OTA_HOSTNAME);
        ArduinoOTA.setPassword(ECL_OTA_PASSWORD);
        ArduinoOTA.onStart([]()
                           { ECL::log("OTA Start"); });
        ArduinoOTA.onEnd([]()
                         { ECL::log("\nOTA End"); });
        ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                              { ECL::logf("Progress: %u%%\r", (progress / (total / 100))); });
        ArduinoOTA.onError([](ota_error_t error)
                           { ECL::logf("Error[%u]\n", error); });
        ArduinoOTA.begin();
        ECL::log("OTA Ready");
#endif
    }

    // --- Loop Handler ---
    inline void loop()
    {
// Handle OTA
#if defined(ECL_OTA_HOSTNAME)
        ArduinoOTA.handle();
#endif

// Handle Telnet
#if defined(ECL_TELNET_PORT)
        if (telnetServer.hasClient())
            telnetClient = telnetServer.available();
#endif
// Handle MQTT Reconnection & Loop
#if defined(ECL_MQTT_SERVER)
        if (!mqttClient.connected())
        {
            if (mqttClient.connect(ECL_MQTT_CLIENT))
            {
                for (const auto &sub : _eclSubscriptions)
                    mqttClient.subscribe(sub.topic.c_str());
                ECL::log("MQTT Reconnected & Subscribed");
            }
        }
        else
        {
            mqttClient.loop();
        }
#endif
        for (const auto &handler : _eclLoopHandlers)
            handler();
    }
}