#pragma once
#include <Arduino.h>
#include "MessageHeaders.h"
#include "DeviceAddress.h"

class DataStore
{
  static uint8_t s_uNextId;

public:
  static void Send(uint32_t uSenderId, byte DataContext, byte DataChannel, void const * pData, uint8_t uDataSize);

private:
  static uint16_t CalculateChecksum(CMessageHeader const &rHeader, uint8_t const * pData, uint8_t uDataSize);
  static uint16_t CalculateChecksum(uint16_t uSeed, uint8_t const *pStart, uint8_t uLength);
  static void WriteHex(const void *pData, uint8_t uLength);
  static void WriteHex(uint8_t uData);
  
};

