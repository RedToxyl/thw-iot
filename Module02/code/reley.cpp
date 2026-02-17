#include <Arduino.h>

#define BTN D1
#define RELEY D6

boolean btn = false;

void setup()
{
  Serial.begin(115200);
  pinMode(RELEY, OUTPUT);
  pinMode(BTN, INPUT_PULLUP);

  digitalWrite(RELEY, LOW);

  Serial.flush();
}

void reley()
{
  digitalWrite(RELEY, HIGH);
  delay(300);
  digitalWrite(RELEY, LOW);
}

void loop()
{
  // ===== loop version =====
  // reley();
  // delay(5000);

  // ===== btn version =====
  unsigned long t = millis();
  boolean v = digitalRead(BTN) == HIGH;
  if (btn != v)
  {
    btn = v;
    Serial.printf("[%lu] Button is %s\n", t, btn ? "HIGH" : "LOW");
    if (v)
    {
      reley();
    }
  }
}