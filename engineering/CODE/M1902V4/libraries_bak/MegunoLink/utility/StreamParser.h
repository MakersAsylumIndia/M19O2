#pragma once
#include <Arduino.h>
#include "CommandDispatcherBase.h"

namespace MLP
{
  class StreamParser
  {
  protected:
    MLP::CommandDispatcherBase *m_pCommandHandler;

    // The stream that we are parsing for commands. 
    // Protected so that network command handlers (such as TCPCommandHandler)
    // can update with connection. If null, commands are not parsed. 
    // Be sure to reset the parser whenever the source is changed. 
    Stream *m_pSource;

    // Characters that signal start and end of message. 
    // The buffer is reset when a start character is received. Commands
    // are dispatched when the end character is received. 
    char m_chStartOfMessage, m_chEndOfMessage;

  private:
    // A buffer to collect commands. Buffer, and size, is provided. 
    char * const m_pchBuffer;
    const unsigned m_uMaxBufferSize;

    // Index where next character will be placed in the buffer. 
    unsigned m_uNextCharacter;

    // True iff an overflow has occured. Commands will not
    // be dispatched from buffers that overflow. 
    bool m_bOverflow;

  protected:
    StreamParser(MLP::CommandDispatcherBase &rCommandHandler, char *pchReceiveBuffer, unsigned uBufferSize, 
      Stream &rSourceStream = Serial, char chStartOfMessage = '!', char chEndOfMessage = '\r');
    StreamParser(char *pchReceiveBuffer, unsigned uBufferSize);

    void DispatchMessage();

  public:
    void Process();
    void Reset(); 
  };
}
