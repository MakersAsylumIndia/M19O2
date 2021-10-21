#pragma once
#include <Arduino.h>

class MegunoLinkProtocol
{
  // A string in flash memory on boards that support this. 
  PGM_P m_pfchContext;

  const void *m_ChannelName;
  const bool m_bFlashString;

protected:
  Print &m_rDestination;

protected:
  MegunoLinkProtocol(const __FlashStringHelper *Context);
  MegunoLinkProtocol(const __FlashStringHelper *Context, const char *Channel);
  MegunoLinkProtocol(const __FlashStringHelper *Context, const __FlashStringHelper *Channel);

  MegunoLinkProtocol(const __FlashStringHelper *Context, Print &rDestination);
  MegunoLinkProtocol(const __FlashStringHelper *Context, const char *Channel, Print &rDestination);
  MegunoLinkProtocol(const __FlashStringHelper *Context, const __FlashStringHelper *Channel, Print &rDestination);

  void SendDataHeader(const __FlashStringHelper* pfstrCommand);
  void SendDataTail();

  // Some boards don't put strings into flash. They redefine F() macro
  // as empty, which causes these overrides to be called. Here we assume
  // if this override is called then the context is not a flash string either. 
  MegunoLinkProtocol(const char* Context);
  MegunoLinkProtocol(const char* Context, const char* Channel);
  MegunoLinkProtocol(const char* Context, const __FlashStringHelper* Channel);

  MegunoLinkProtocol(const char* Context, Print& rDestination);
  MegunoLinkProtocol(const char* Context, const char* Channel, Print& rDestination);
  MegunoLinkProtocol(const char* Context, const __FlashStringHelper* Channel, Print& rDestination);


  void SendDataHeader(const char *pfstrCommand);

public:
  void Attach(Print& rDestination);

};

