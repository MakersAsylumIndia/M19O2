#include "Audio.h"

Audio::Audio(Print& rDestination)
  : MegunoLinkProtocol(F("AUDIO"), rDestination)
{
}

void Audio::PlayAudioClip(const char* ClipId)
{
  PlayClip(ClipId);
}

void Audio::PlayAudioClip(const __FlashStringHelper* ClipId)
{
  PlayClip(ClipId);
}

void Audio::PlayAudioClip(const String& ClipId)
{
  PlayClip(ClipId.c_str());
}
