#include <ECL.h>
#include <SPI.h>
#include <MFRC522.h>

#define BZZ_PIN D1

#define NOTE_A3 220
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G4 392

#define RST_PIN D0
#define SDA_PIN D8

MFRC522 mfrc522(SDA_PIN, RST_PIN);

int notes[] = { NOTE_A3,
                NOTE_B3,
                NOTE_C4,
                NOTE_D4,
                NOTE_E4,
                NOTE_F4,
                NOTE_G4 };

int note = -1;

void mqttCallback(const char *topic, const char *payload)
{
    if (strcmp(topic, "buzzer") == 0)
    {
        note = atoi(payload);
    }
}

void setup()
{
    ECL::begin();
    SPI.begin();
    mfrc522.PCD_Init();
    ECL::mqttSubscribe("buzzer", mqttCallback);
}

void loop()
{
    ECL::loop();
    if (note == -1) {
        noTone(BZZ_PIN);
    }
    else {
        tone(BZZ_PIN, notes[note]);
    }
    
    delay(10);

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