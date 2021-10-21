#pragma once
#include "MegunoLinkProtocol.h"

class Audio : public MegunoLinkProtocol
{
public:
  Audio(Print& rDestination = Serial);

  template<typename TContent> void Speak(TContent Content)
  {
    SendDataHeader(F("SPK"));
    m_rDestination.print(Content);
    SendDataTail();
  }

  void PlayAudioClip(const char* ClipId);
  void PlayAudioClip(const __FlashStringHelper* ClipId);
  void PlayAudioClip(const String& ClipId);

  void Stop()
  {
    SendDataHeader(F("STP"));
    SendDataTail();
  }

private:
  template<typename TId> void PlayClip(const TId &ClipId)
  {
    SendDataHeader(F("PLA"));
    m_rDestination.print(ClipId);
    SendDataTail();
  }
};