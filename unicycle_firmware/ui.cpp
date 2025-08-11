#include "stdinc.h"
#include <FastLED.h>

CRGB leds[1];

void setupUI()
{
  FastLED.addLeds<WS2812B, 47, RGB>(leds, 1);
  ledColor(64, 0, 0);
  pinMode(0, INPUT_PULLUP);
}

void ledColor(uint8_t r, uint8_t g, uint8_t b)
{
  leds[0] = CRGB(r, g, b);
  FastLED.show();
}

void loopUI()
{
  static int8_t dbb = 0;
  int8_t b = digitalRead(0) * 2 - 1;
  dbb += b;
  dbb = LIMIT(dbb, -10, 10);

  static int8_t dbj = 0;
  dbj += (joystickReport.buttons == 3072 ? 1 : -1);
  dbj = LIMIT(dbj, -10, 10);
  if ((dbb == 0 && b == -1) || (dbj == 0 && joystickReport.buttons == 3072))
  {
    if (!wifiOn)
    {
      setupWifi();
      setupOta();
      setupInputOutput();
    }
    else
    {
      disconnectWifi();
    }
  }
}
