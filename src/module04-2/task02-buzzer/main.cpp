#include <ECL.h>

#define BZZ_PIN D8

#define NOTE_A3 220
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G4 392

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
    
    delay(100);
}