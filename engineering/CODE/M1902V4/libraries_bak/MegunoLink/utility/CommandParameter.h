#pragma once
#include <Arduino.h>

class CommandParameter
{
  // Buffer of parameters. As we work through each one, it gets null terminated. 
  char *m_pchBuffer;

  // Offset to next parameter in buffer. 
  uint8_t m_uNextParameter; 

  // The stream that the parameter came from (for replies, for example)
  Print &m_rSourceStream;

public:
  CommandParameter(Print &rSourceStream, char *pchBuffer, uint8_t nFirstParameter);

  Print &GetSource() { return m_rSourceStream; }

  const char *NextParameter();
  const char *RemainingParameters();

  int NextParameterAsInteger(int nDefault = -1);
  long NextParameterAsLong(long nDefault = -1);
  unsigned long NextParameterAsUnsignedLong(unsigned long nDefault = -1);
  double NextParameterAsDouble(double fDefault = 0.0);
};

