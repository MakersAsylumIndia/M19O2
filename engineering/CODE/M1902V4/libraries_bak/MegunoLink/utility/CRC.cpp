#include <stdint.h>

#if defined(__AVR__)

#include <util/crc16.h>

#else

// Backup CRC algorithm where not supported by native library. Based on 
// http://stackoverflow.com/questions/10564491/function-to-calculate-a-crc16-checksum
uint16_t _crc16_update(uint16_t crc, uint8_t data) {
  uint8_t x;

  x = crc >> 8 ^ data;
  x ^= x >> 4;
  crc = (crc << 8) ^ ((uint16_t)(x << 12)) ^ ((uint16_t)(x << 5)) ^ ((uint16_t)x);

  return crc;
}

#endif
