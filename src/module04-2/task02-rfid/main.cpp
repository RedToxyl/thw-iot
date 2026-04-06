#include <ECL.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN D0
#define SDA_PIN D8

MFRC522 mfrc522(SDA_PIN, RST_PIN);

void setup()
{
    ECL::begin();
    SPI.begin();
    mfrc522.PCD_Init();
}

void loop()
{
    ECL::loop();
    delay(500);

    if (!mfrc522.PICC_IsNewCardPresent())
    {
        return;
    }
    if (!mfrc522.PICC_ReadCardSerial())
    {
        return;
    }
    String uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
        uid += String(mfrc522.uid.uidByte[i], HEX);
    }
    ECL::mqttPublish("rfid", uid.c_str());
    ECL::log.println("Card UID: " + uid);
}