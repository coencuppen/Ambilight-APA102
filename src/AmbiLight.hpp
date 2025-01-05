#ifndef LIGHT_EFFECTS_HPP
#define LIGHT_EFFECTS_HPP

#define BRIGHTNESS 255
#define NUM_OF_LED_ANIMATION 1
#define NUM_OF_ANIMATION_SPEED 8 // 8ms
#define APA102_BRIGHTNESS_RANGE 31

#define STARTUP_ANIMATION_COLOR 216,129,85


#include <util/delay.h>

#include "APA102.hpp"

uint8_t prefix[] = {'A', 'd', 'a'}, hi, lo, chk, i;

class AmbiLight {
private:
  uint16_t numOfLeds;
  APA102 *Leds;

  void startupAnimation() {
    // Leds->clear();
    // for(unsigned int i = 0; i < NUM_OF_LED_ANIMATION; i++){
    //   for(unsigned int i = 0; i < numOfLeds; i++){
    //     Leds->startFrame();
    //     for(unsigned int j = 0; j < numOfLeds; j++){
    //       if(j == i){
    //         Leds->sendColor(STARTUP_ANIMATION_COLOR, 127);
    //       } else {
    //         Leds->sendColor(0, 0, 0, 0);
    //       }
    //     }
    //     Leds->endFrame();
    //     _delay_ms(NUM_OF_ANIMATION_SPEED);
    //   }
    // }

    Leds->clear();
    Leds->startFrame();

    for(unsigned int i = 0; i < APA102_BRIGHTNESS_RANGE; i++){ 
      Leds->sendColorAll(STARTUP_ANIMATION_COLOR, i);
      _delay_ms(30);
    }
    _delay_ms(900);

    for(unsigned int i = APA102_BRIGHTNESS_RANGE; i > 0; i--){
      Leds->sendColorAll(STARTUP_ANIMATION_COLOR, i);
      _delay_ms(25);
    }

    Leds->clear();
  }

public:
  AmbiLight(uint16_t numOfLeds) : numOfLeds(numOfLeds) {
    Leds = new APA102{numOfLeds + 1};
    startupAnimation();
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