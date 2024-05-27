#ifndef SPI_HPP
#define SPI_HPP

#include <SPI.h>

namespace spi {
  void sendByte(uint8_t data) {
    SPDR = data; // Load data into SPI data register
    while (!(SPSR & (1 << SPIF)))
      ; // Wait until transmission complete
  }
} // namespace SPI

#endif
