#include "StreamParser.h"

using namespace MLP;

StreamParser::StreamParser( MLP::CommandDispatcherBase &rCommandHandler, 
                           char *pchReceiveBuffer, 
                           unsigned uBufferSize, 
                           Stream &rSourceStream /*= Serial*/, 
                           char chStartOfMessage /*= '!'*/, 
                           char chEndOfMessage /*= '\r'*/ )
                           : m_pCommandHandler(&rCommandHandler)
                           , m_pSource(&rSourceStream)
                           , m_chStartOfMessage(chStartOfMessage)
                           , m_chEndOfMessage(chEndOfMessage)
                           , m_pchBuffer(pchReceiveBuffer)
                           , m_uMaxBufferSize(uBufferSize)
{
  Reset();
}

StreamParser::StreamParser(char *pchReceiveBuffer, unsigned uBufferSize)
  : m_pchBuffer(pchReceiveBuffer)
  , m_uMaxBufferSize(uBufferSize)
{
  Reset();
}


void StreamParser::Process()
{
  while (m_pSource != NULL && m_pSource->available() > 0)
  {
    char chNext = m_pSource->read();

    if (chNext == m_chStartOfMessage)
    {
      Reset();
      m_pchBuffer[0] = chNext; // the start character serves as a flag that this message is valid. 
      ++m_uNextCharacter;
    }
    else if (chNext == m_chEndOfMessage)
    {
      if (m_uNextCharacter > 0 && m_pchBuffer[0] == m_chStartOfMessage)
      {
        if (!m_bOverflow)
        {
          // We have a valid message. Dispatch it. 
          m_pchBuffer[m_uNextCharacter] = '\0';
          DispatchMessage();
          Reset();
        }
        else
        {
          Serial.println(F("Ovrflw"));
        }
      }
    }
    else
    {
      // Store the character and advance the buffer. If there isn't enough
      // room for the message to be terminated by a null, flag overflow. 
      if (!m_bOverflow)
      {
        m_pchBuffer[m_uNextCharacter++] = chNext;
        m_bOverflow = m_uNextCharacter >= (m_uMaxBufferSize - 1);
      }
    }
  }
}

void StreamParser::DispatchMessage()
{
  m_pCommandHandler->DispatchCommand(m_pchBuffer + 1, *m_pSource); // First character is start of message signal. 
} 

void StreamParser::Reset()
{
  m_uNextCharacter = 0;
  m_bOverflow = false;
}