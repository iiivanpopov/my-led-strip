#include <FastLED.h>

#define LED_PIN 5
#define LED_NUM 300

CRGB leds[LED_NUM];

int brightness = 50;
bool pulse = false;
bool running = false;
bool fadeDirection = true;

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_NUM);
  Serial.begin(115200);
}

void loop() {
  if (Serial.available()) {
    int mode = Serial.parseInt();
    if (Serial.read() == '\n') handleSerialCommand(mode);
  }

  if (pulse) pulsing();
  if (running) runningRainbow();
}

void handleSerialCommand(int mode) {
  switch (mode) {
    case 1:  // Rainbow
      fill_rainbow(leds, LED_NUM, millis() / 100);
      FastLED.show();
      break;
    case 2:  // Pink
      fill_solid(leds, LED_NUM, CRGB(0xed0e9b));
      FastLED.show();
      break;
    case 3:  // Red
      fill_solid(leds, 300, CRGB::Red);
      FastLED.show();
      break;
    case 4:  // Purple
      fill_solid(leds, LED_NUM, CRGB(0x8519d7));
      FastLED.show();
      break;
    case 5:  // Turn off
      FastLED.clear();
      FastLED.show();
      break;
    case 6:  // Turn on
      FastLED.setBrightness(brightness);
      FastLED.show();
      break;
    case 7:  // Toggle pulse
      pulse = !pulse;
      break;
    case 8:  // Toggle running rainbow
      running = !running;
      break;
    case 9:  // Decrease brightness
      brightness -= 4;
      if (brightness < 0) brightness = 0;
      FastLED.setBrightness(brightness);
      FastLED.show();
      break;
    case 10:  // Increase brightness
      brightness += 4;
      if (brightness > 255) brightness = 255;
      FastLED.setBrightness(brightness);
      FastLED.show();
      break;
  }
}

void pulsing() {
  if (fadeDirection) {
    EVERY_N_MILLISECONDS(5) {
      FastLED.setBrightness(brightness);
      FastLED.show();
      brightness += 1;
      if (brightness == 255) fadeDirection = !fadeDirection;
    }
  } else {
    EVERY_N_MILLISECONDS(8) {
      FastLED.setBrightness(brightness);
      FastLED.show();
      brightness -= 1;
      if (brightness == 0) fadeDirection = !fadeDirection;
    }
  }
}


void runningRainbow() {
  EVERY_N_MILLISECONDS(10) {
    fill_rainbow(leds, LED_NUM, millis() / 10);
    FastLED.show();
  }
}
