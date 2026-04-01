#include <ECL.h>
#include <NewPing.h>

#define TRIG_PIN 27
#define ECHO_PIN 25
#define MAX_DISTANCE 200 // in cm

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

void setup()
{
    ECL::begin();
}

void loop()
{
    ECL::loop();
    unsigned int distance = sonar.ping_cm();
    ECL::log.printf("Distance: %u cm\n", distance);
    if (0 < distance && distance < MAX_DISTANCE){
        char buffer[10];
        snprintf(buffer, sizeof(buffer), "%u", distance);
        ECL::mqttPublish("sonar", buffer);
    }
    delay(150);
}