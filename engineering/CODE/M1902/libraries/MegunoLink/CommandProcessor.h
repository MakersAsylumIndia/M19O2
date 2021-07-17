#pragma once
#include <Arduino.h>
#include "CommandDispatcher.h"
#include "utility/StreamParser.h"

template <int CP_SERIAL_BUFFER_SIZE = 30> class CommandProcessor : public MLP::StreamParser
{
  // Buffer for data received.
  char m_achBuffer[CP_SERIAL_BUFFER_SIZE];

public:
  CommandProcessor(MLP::CommandDispatcherBase &rCommandHandler, Stream &rSourceStream = Serial, char chStartOfMessage = '!', char chEndOfMessage = '\r')
    : StreamParser(rCommandHandler, m_achBuffer, CP_SERIAL_BUFFER_SIZE, rSourceStream, chStartOfMessage, chEndOfMessage)
  {
  }
};

