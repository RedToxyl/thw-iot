#include <ECL.h>
#include <U8g2lib.h>

U8G2_SSD1306_64X48_ER_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void send(const char* msg, int timeout = 1500) {
    u8g2.drawStr(5, 30, msg);
    u8g2.sendBuffer();
    delay(timeout);
    u8g2.clearBuffer();
    u8g2.sendBuffer();
}

void mqttCallback(const char *topic, const char *payload)
{
    if (strcmp(topic, "display") == 0)
    {
        send(payload);
    }
}

void setup()
{
    ECL::begin();
    ECL::mqttSubscribe("display", mqttCallback);

    // Initialize the display
    u8g2.begin();
    // Clear the display buffer
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_5x7_tr); // Choose a suitable font
    send("OLED Ready!", 2000);
}

void loop()
{
    ECL::loop();
    delay(100);
}