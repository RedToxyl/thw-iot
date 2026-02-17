#include <Arduino.h>

#define LED_A D3
#define LED_B D4
#define LED_C D6
#define LED_D D2
#define BTN D5

// blinking interval
const long INTERVAL_A = 1000;
const long INTERVAL_B = 700;

// states
unsigned long time_a = 0;
unsigned long time_b = 0;
boolean btn = false;

void changeLED(uint8_t led)
{
  digitalWrite(led, digitalRead(led) == LOW ? HIGH : LOW);
}

void setup()
{
  Serial.begin(115200);
  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(LED_C, OUTPUT);
  pinMode(BTN, INPUT_PULLUP);

  digitalWrite(LED_A, LOW);
  digitalWrite(LED_B, LOW);
  digitalWrite(LED_C, HIGH);

  Serial.flush();
}

void loop()
{
  unsigned long t = millis();

  if (t - time_a >= INTERVAL_A)
  {
    time_a = t;
    changeLED(LED_A); //for ssynchronously blinking
  }
  if (t - time_b >= INTERVAL_B)
  {
    time_b = t;
    changeLED(LED_B);
    changeLED(LED_C); //for synchronously blinking (one on other off)
  }
  boolean v = digitalRead(BTN) == HIGH;
  if (btn != v)
  {
    btn = v;
    Serial.printf("[%lu] Button is %s\n", t, btn ? "HIGH" : "LOW");
    if (v) changeLED(LED_C); // change state on btn press
  }
}