#pragma once

struct CMessageHeader
{
  enum EType  
  { 
    TYPE_Command = '!', 
    TYPE_Reply = '@' 
  } __attribute__((__packed__));
  
  enum EContext
  { 
    CTX_DataTransport = 'A',
  } __attribute__((__packed__));

  enum EConstants
  {
    ChecksumSeed = 0x50
  } __attribute__((__packed__));

  uint16_t Checksum;  // entire message content, excluding checksum bytes. Must be at start.
  EType Type;
  uint8_t Id;
  uint32_t SenderAddress;
  uint16_t TimeStamp; // For debugging.
  EContext Context;
  uint16_t Command;
} __attribute__((__packed__));

