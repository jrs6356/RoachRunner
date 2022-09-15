#include <SPI.h>

#define dc    A0    // Display/command mode
#define sda   51    // MOSI: D0-D7
#define cs1b  35    // Chip select
#define rst   37    // Reset
#define scl   52    // Serial clock

// Max speed: 10 MHz
// Read type: Rising
// MSB first
// Clock idle High
// SPI_MODE3

SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE3));

void setup() {
  SPI.begin(53)

}

void loop() {
  // put your main code here, to run repeatedly:

}
