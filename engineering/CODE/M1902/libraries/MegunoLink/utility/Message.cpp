#include "Message.h"

Message::Message(Print& rDestination)
  : MegunoLinkProtocol(F("MESSAGE"), rDestination)
  , m_Destination(Text)
{

}

Message::Message(const char *channelName /*= NULL*/, Print &rDestination)
  : MegunoLinkProtocol(F("MESSAGE"), channelName, rDestination)
  , m_Destination(Text)
{

}

Message::Message(const __FlashStringHelper* channelName, Print& rDestination)
  : MegunoLinkProtocol(F("MESSAGE"), channelName, rDestination)
  , m_Destination(Text)
{

}

Message::Message(MessageDestination Destination, Print& rDestination)
  : MegunoLinkProtocol(F("MESSAGE"), rDestination)
  , m_Destination(Destination)
{

}

Message::Message(MessageDestination Destination, const char *channelName /*= NULL*/, Print &rDestination)
  : MegunoLinkProtocol(F("MESSAGE"), channelName, rDestination)
  , m_Destination(Destination)
{

}

Message::Message(MessageDestination Destination, const __FlashStringHelper *channelName, Print &rDestination)
  : MegunoLinkProtocol(F("MESSAGE"), channelName, rDestination)
  , m_Destination(Destination)
{

}

void Message::Begin()
{
  switch (m_Destination)
  {
  case Text:
    SendDataHeader(F("DATA"));
    break;
  case Speak:
    SendDataHeader(F("SPEAK"));
    break;
  case TextAndSpeak:
    SendDataHeader(F("SPEAK+DATA"));
    break;
  }
}

void Message::End()
{
  SendDataTail();
}

void Message::Send( const char *Message )
{
  Begin();
  m_rDestination.print(Message);
  SendDataTail();
}

void Message::Send( const __FlashStringHelper *Message )
{
  Begin();
  m_rDestination.print(Message);
  SendDataTail();
}

void Message::Clear()
{
  SendDataHeader(F("CLEAR"));
  SendDataTail();
}

void Message::LogTo(const __FlashStringHelper * Filename)
{
  SendDataHeader(F("F"));
  m_rDestination.print(Filename);
  SendDataTail();
}

void Message::LogTo(const char * Filename)
{
  SendDataHeader(F("F"));
  m_rDestination.print(Filename);
  SendDataTail();
}

void Message::LogTo(String &Filename)
{
  SendDataHeader(F("F"));
  m_rDestination.print(Filename.c_str());
  SendDataTail();
}

void Message::StopLogging()
{
  SendDataHeader(F("X"));
  SendDataTail();
}

void Message::StartLogging()
{
  SendDataHeader(F("F"));
  SendDataTail();
}

void Message::Flush()
{
  SendDataHeader(F("W"));
  SendDataTail();
}

void Message::SendSeparator()
{
  m_rDestination.print(F(": "));
}


