#include "DataStore.h"
#include "utility/CRC.h"

uint8_t DataStore::s_uNextId = 0;

void DataStore::Send(uint32_t uSenderId, byte DataContext, byte DataChannel, void const * pData, uint8_t uDataSize)
{
  CMessageHeader Header;

  Header.Type = CMessageHeader::TYPE_Command;
  Header.Id = s_uNextId++;
  Header.SenderAddress = uSenderId;
  Header.TimeStamp = millis() & 0xffff;
  Header.Context = CMessageHeader::CTX_DataTransport;
  Header.Command = DataContext;
  Header.Command = (Header.Command << 8) | (DataChannel + 0x30);
  Header.Checksum = CalculateChecksum(Header, (uint8_t const *)pData, uDataSize);

  Serial.print((char)Header.Type);
  WriteHex(Header.Id);
  Serial.print('a');
  WriteHex((uint8_t *)&Header.SenderAddress, sizeof(Header.SenderAddress));
  Serial.print('t');
  WriteHex((uint8_t *)&Header.TimeStamp, sizeof(Header.TimeStamp));
  Serial.print('#');
  WriteHex((uint8_t *)&Header.Checksum, sizeof(Header.Checksum));
  Serial.print('-');
  Serial.print((char)(Header.Context));
  Serial.print((char)(Header.Command&0xff));
  Serial.print((char)(Header.Command>>8));
  Serial.print('(');
  Serial.print(uDataSize);
  Serial.print(')');
  WriteHex(pData, uDataSize);
  
  Serial.println();
}

uint16_t DataStore::CalculateChecksum(uint16_t uSeed, uint8_t const *pStart, uint8_t uLength)
{
  while(uLength--)
  {
    uSeed = _crc16_update(uSeed, *pStart++);
  }

  return uSeed;
}

uint16_t DataStore::CalculateChecksum(CMessageHeader const &rHeader, uint8_t const * pData, uint8_t uDataSize)
{
  uint16_t uChecksum = CalculateChecksum(CMessageHeader::ChecksumSeed, (uint8_t const *) &rHeader + sizeof(uint16_t), sizeof(rHeader) - sizeof(uint16_t));
  uChecksum = CalculateChecksum(uChecksum, pData, uDataSize);

  return uChecksum;
}

void DataStore::WriteHex(uint8_t uData)
{
  unsigned uNibble;
  char chOut;

  
  // High nibble. 
  uNibble = (uData>>4)&0x0f;
  chOut = uNibble + ((uNibble < 10) ? '0' : ('A' - 10));
  Serial.write(chOut);

  // Low nibble. 
  uNibble = uData&0x0f;
  chOut = uNibble + ((uNibble < 10) ? '0' : ('A' - 10));
  Serial.write(chOut);
}

void DataStore::WriteHex(const void *pSource, uint8_t uLength)
{
  const uint8_t *pData = (uint8_t*) pSource;

  while (uLength--)
    WriteHex(*pData++);
}

