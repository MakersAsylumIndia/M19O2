#pragma once
#include "MegunoLinkProtocol.h"

class Message : public MegunoLinkProtocol
{
public:
  // Flags to select destination for messages
  enum MessageDestination
  {
    Text = 0x01,

    Speak = 0x02,

    TextAndSpeak = 0x03,
  };

private:
  
  const MessageDestination m_Destination;

public:
  Message(Print& rDestination = Serial);
  Message(const char *channelName, Print &rDestination = Serial);
  Message(const __FlashStringHelper *channelName, Print &rDestination = Serial);

  Message(MessageDestination Destination, Print& rDestination = Serial);
  Message(MessageDestination Destination, const char* channelName = NULL, Print& rDestination = Serial);
  Message(MessageDestination Destination, const __FlashStringHelper *channelName, Print &rDestination = Serial);

  void Begin();
  void End();

  void Send(const char *Message);
  void Send(const __FlashStringHelper *Message);

  template<class TValue> void Send(TValue Value)
  {
    Begin();
    m_rDestination.print(Value);
    SendDataTail();
  }

  template<class TValue> void Send(long Time, TValue Value)
  {
    Begin();
    m_rDestination.print(Time);
    SendSeparator();
    m_rDestination.print(Value);
    SendDataTail();
  }

  template<class TValue> void Send(const char *Label, TValue Value)
  {
    Begin();
    m_rDestination.print(Label);
    SendSeparator();
    m_rDestination.print(Value);
    SendDataTail();
  }

  template<class TValue> void Send(const __FlashStringHelper *Label, TValue Value)
  {
    Begin();
    m_rDestination.print(Label);
    SendSeparator();
    m_rDestination.print(Value);
    SendDataTail();
  }

  void Clear();

  void LogTo(const __FlashStringHelper *Filename);
  void LogTo(String &Filename);
  void LogTo(const char *Filename);
  void StartLogging();
  void StopLogging();
  void Flush();

private:
  void SendSeparator();
};

