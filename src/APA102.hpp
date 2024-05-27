#ifndef APA102_HPP
#define APA102_HPP

#include "SPI.hpp"

#define DATA_PIN 11  // MOSI pin
#define CLOCK_PIN 13 // SCK pin

#define DDRD_REG (*(volatile uint8_t *)(0x2A)) // Data Direction Register for Port D

class APA102 {
private:
  const size_t size; // number of LED's

public:
  APA102(size_t size) : size(size) {

    DDRD_REG |= (1 << DATA_PIN);  // Set pin as an output
    DDRD_REG |= (1 << CLOCK_PIN); // Set pin as an output

    SPI.begin();
    SPI.setClockDivider(
        SPI_CLOCK_DIV2); // Set SPI clock to 8MHz (maximum for APA102)

    clear();
  }

  void clear() const {
    spi::sendByte(0x00);
    spi::sendByte(0x00);
    spi::sendByte(0x00);
    spi::sendByte(0x00);

    for (uint8_t led = 0; led < size; ++led) {
      spi::sendByte(0xE0 | 255);
      spi::sendByte(0x00);
      spi::sendByte(0x00);
      spi::sendByte(0x00);
    }

    spi::sendByte(0xFF);
    spi::sendByte(0xFF);
    spi::sendByte(0xFF);
    spi::sendByte(0xFF);
  }

  void sendColor(const uint8_t &red, const uint8_t &green, const uint8_t &blue, uint8_t brightness = 255){
    spi::sendByte( 0xE0 | brightness);   // Start frame for a LED with highest 3 bits set to 111
    spi::sendByte(blue); 
    spi::sendByte(green);
    spi::sendByte(red);  
  }

  void sendColorAll(const uint8_t &red, const uint8_t &green, const uint8_t &blue, uint8_t brightness = 255) const {
    clear();

    spi::sendByte(0x00);
    spi::sendByte(0x00);
    spi::sendByte(0x00);
    spi::sendByte(0x00);

    for (uint8_t led = 0; led < size; ++led) {
      spi::sendByte( 0xE0 | brightness);   // Start frame for a LED with highest 3 bits set to 111
      spi::sendByte(blue); 
      spi::sendByte(green);
      spi::sendByte(red);  
    }

    spi::sendByte(0xFF);
    spi::sendByte(0xFF);
    spi::sendByte(0xFF);
    spi::sendByte(0xFF);
  }
};

#endif