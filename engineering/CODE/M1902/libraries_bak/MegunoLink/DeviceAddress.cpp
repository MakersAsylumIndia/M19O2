#include <Arduino.h>
#include "DeviceAddress.h"

#if defined(__AVR__) // Only AVR devices implement EEPROMs for Arduino. 
#include <avr/eeprom.h>

uint32_t GetDeviceAddress(uint8_t const * pLocation /*= DEFAULT_DEVICE_ID_ADDRESS*/)
{
  uint32_t uId;
  eeprom_read_block(&uId, pLocation, sizeof(uId));
  return uId;
}
#endif



void WriteMelvynFormatId(uint32_t uId)
{
  uint8_t u1 = ((uId & 0x03) << 3) | ((uId >> 13) & 0x07);
  uint8_t u2 = (uId >> 8) & 0x1f;
  char ch1 = (u1 == 0 || u1 > 26) ? ' ' : u1 - 1 + 'A';
  char ch2 = (u2 == 0 || u2 > 26) ? ' ' : u2 - 1 + 'A';
  uint16_t uNumericId = uId >> 16;

  Serial.print(ch1);
  Serial.print(ch2);
  Serial.print('-');
  Serial.print(uNumericId);
  Serial.println();
}
