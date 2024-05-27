#ifndef LIGHT_EFFECTS_HPP
#define LIGHT_EFFECTS_HPP

#define BRIGHTNESS 255

#include <util/delay.h>

#include "APA102.hpp"

uint8_t prefix[] = {'A', 'd', 'a'}, hi, lo, chk, i;

class AmbiLight {
private:
  uint16_t numOfLeds;
  APA102 *Leds;

public:
  AmbiLight(uint16_t numOfLeds) : numOfLeds(numOfLeds) {
    Leds = new APA102{numOfLeds + 1};

    Leds->sendColorAll(255, 0, 0);
    _delay_ms(500);
    Leds->sendColorAll(0, 255, 0);
    _delay_ms(500);
    Leds->sendColorAll(0, 0, 255);
    _delay_ms(500);
    Leds->clear();
  }

  void ambiLoop() {
    for (i = 0; i < sizeof(prefix); ++i) {
      while (!Serial.available())
        ;
      // Check next byte in Magic Word
      if (prefix[i] == Serial.read())
        continue;
      // otherwise, start over
      i = 0;
    }

    // Hi, Lo, Checksum
    while (!Serial.available())
      ;
    hi = Serial.read();
    while (!Serial.available())
      ;
    lo = Serial.read();
    while (!Serial.available())
      ;
    chk = Serial.read();

    // If checksum does not match, go back to wait
    if (chk != (hi ^ lo ^ 0x55)) {
      return;
    }

    spi::sendByte(0x00);
    spi::sendByte(0x00);
    spi::sendByte(0x00);
    spi::sendByte(0x00);

    // Read the transmission data and set LED values
    for (uint16_t led = 0; led < numOfLeds; led++) {
      uint8_t r, g, b;
      while (!Serial.available())
        ;
      r = Serial.read();
      while (!Serial.available())
        ;
      g = Serial.read();
      while (!Serial.available())
        ;
      b = Serial.read();

      Leds->sendColor(r, g, b);
    }

    spi::sendByte(0xFF);
    spi::sendByte(0xFF);
    spi::sendByte(0xFF);
    spi::sendByte(0xFF);
  };
};

#endif