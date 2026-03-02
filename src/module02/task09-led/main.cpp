//Install [Adafruit_NeoPixel_Library](https://github.com/adafruit/Adafruit_NeoPixel) first.

#include <Adafruit_NeoPixel.h>

#define LED_NUM 7
#define BRIGHTNESS 0.05

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_NUM, D4, NEO_GRB + NEO_KHZ800);



void setup() {
    leds.begin(); // This initializes the NeoPixel library.
}


void led_set(uint8 R, uint8 G, uint8 B) {
    for (int i = 0; i < LED_NUM; i++) {
    leds.setPixelColor(i, leds.Color(R * BRIGHTNESS, G * BRIGHTNESS, B * BRIGHTNESS));
    leds.show();
    delay(50);
    }
}

void loop() {
    led_set(14, 0, 211);
    led_set(75, 0, 130);
    led_set(0, 0, 255);
    led_set(0, 255, 0);
    led_set(255, 255, 0);
    led_set(255, 127, 0);
    led_set(255, 0, 0);
}
