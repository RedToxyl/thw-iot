#define ECL_MQTT_SERVER "192.168.14.1"

#include "ECL.h"
#include "Button.h"
#include <map>
#include <U8g2lib.h>

ECL::Button passwordBtn(17);
U8G2_SSD1306_64X48_ER_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

const unsigned long DASH_THRESHOLD = 250;
const unsigned long LETTER_GAP = 1000;
const unsigned long SUBMIT_GAP = 5000;

bool isResetting = false;
unsigned long pressStartTime = 0;
String currentMorse = "";
String finalPassword = "";

unsigned long lastReleaseTime = 0;

bool displayNeedsUpdate = true;
bool showUploaded = false;
unsigned long uploadMessageTimer = 0;
String lastUploadedPassword = "";
const unsigned long UPLOAD_DISPLAY_TIME = 4000;

std::map<String, char> morseAlphabet = {
    // Letters
    {".-", 'A'}, {"-...", 'B'}, {"-.-.", 'C'}, {"-..", 'D'}, {".", 'E'}, 
    {"..-.", 'F'}, {"--.", 'G'}, {"....", 'H'}, {"..", 'I'}, {".---", 'J'}, 
    {"-.-", 'K'}, {".-..", 'L'}, {"--", 'M'}, {"-.", 'N'}, {"---", 'O'}, 
    {".--.", 'P'}, {"--.-", 'Q'}, {".-.", 'R'}, {"...", 'S'}, {"-", 'T'}, 
    {"..-", 'U'}, {"...-", 'V'}, {".--", 'W'}, {"-..-", 'X'}, {"-.--", 'Y'}, {"--..", 'Z'},
    // Numbers
    {"-----", '0'}, {".----", '1'}, {"..---", '2'}, {"...--", '3'}, {"....-", '4'}, 
    {".....", '5'}, {"-....", '6'}, {"--...", '7'}, {"---..", '8'}, {"----.", '9'}
};
void onPasswordReset(char *topic, char *payload)
{
    isResetting = true;
    finalPassword = "";
    currentMorse = "";
    showUploaded = false;
    displayNeedsUpdate = true;
    ECL::log.println("System Ready: Input Morse Code...");
}

void setup()
{
    ECL::begin();
    ECL::mqttSubscribe("password/reset", onPasswordReset);

    u8g2.begin();
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_5x7_tr);
}

void handlePasswordUpdate()
{
    if (!isResetting)
        return;

    bool isPressed = !passwordBtn.isPressed();
    static bool lastState = false;

    // Button just pressed
    if (isPressed && !lastState)
    {
        pressStartTime = millis();
        lastState = true;
    }

    // Button just released
    if (!isPressed && lastState)
    {
        unsigned long duration = millis() - pressStartTime;
        currentMorse += (duration < DASH_THRESHOLD) ? "." : "-";
        lastReleaseTime = millis();
        lastState = false;
        displayNeedsUpdate = true;

        ECL::log.print("Current Buffer: ");
        ECL::log.println(currentMorse);
    }

    // Check for gaps
    if (!isPressed && currentMorse.length() > 0)
    {
        // End of a letter
        if (millis() - lastReleaseTime > LETTER_GAP)
        {
            if (morseAlphabet.count(currentMorse))
            {
                finalPassword += morseAlphabet[currentMorse];
                ECL::log.print("Password so far: ");
                ECL::log.println(finalPassword);
            }
            currentMorse = "";
            displayNeedsUpdate = true;
        }
    }

    // Check for submission timeout
    if (!isPressed && finalPassword.length() > 0 && (millis() - lastReleaseTime > SUBMIT_GAP))
    {
        ECL::mqttPublish("password/value", (char *)finalPassword.c_str());
        ECL::log.println("Password Published!");
        lastUploadedPassword = finalPassword;
        showUploaded = true;
        uploadMessageTimer = millis();
        displayNeedsUpdate = true;

        isResetting = false;
        finalPassword = "";
    }
}

void handleDisplay()
{
    if (showUploaded && (millis() - uploadMessageTimer > UPLOAD_DISPLAY_TIME))
    {
        showUploaded = false;
        displayNeedsUpdate = true;
    }

    if (!displayNeedsUpdate)
        return;
    displayNeedsUpdate = false;

    u8g2.clearBuffer();

    if (showUploaded)
    {
        u8g2.drawStr(0, 10, "SAVED PWD:");
        u8g2.setCursor(0, 25);
        u8g2.print(lastUploadedPassword);
    }
    else if (isResetting)
    {
        u8g2.drawStr(0, 10, "ENTER PWD:");
        u8g2.setCursor(0, 25);
        u8g2.print(finalPassword);
        u8g2.setCursor(0, 40);
        u8g2.print(currentMorse);
    }
    else
    {
        u8g2.drawStr(0, 25, "   WELCOME");
    }

    u8g2.sendBuffer();
}

void loop()
{
    ECL::loop();
    handlePasswordUpdate();
    handleDisplay();
}