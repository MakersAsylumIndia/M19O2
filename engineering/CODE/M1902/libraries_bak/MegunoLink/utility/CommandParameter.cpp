#include "CommandParameter.h"

CommandParameter::CommandParameter(Print &rSourceStream, char *pchBuffer, uint8_t uFirstParameter)
  : m_rSourceStream(rSourceStream)
{
  m_pchBuffer = pchBuffer;
  m_uNextParameter = uFirstParameter;
}

const char *CommandParameter::NextParameter()
{
  if (m_pchBuffer[m_uNextParameter] == '\0')
    return NULL;

  // Null terminate the next parameter
  char *pchParameter = m_pchBuffer + m_uNextParameter;
  char *pchEnd = pchParameter;
  while (*pchEnd != ' ' && *pchEnd != '\0')
  {
    ++pchEnd;
    ++m_uNextParameter;
  }

  if (*pchEnd != '\0')
  {
    ++m_uNextParameter;
    *pchEnd = '\0';
  }

  return pchParameter;
}

const char * CommandParameter::RemainingParameters()
{
  return m_pchBuffer + m_uNextParameter;
}

int CommandParameter::NextParameterAsInteger( int nDefault /*= -1*/ )
{
  const char *pchParameter = NextParameter();
  if (pchParameter == NULL)
    return nDefault;

  return atoi(pchParameter);
}

long CommandParameter::NextParameterAsLong( long nDefault /*= -1*/ )
{
  const char *pchParameter = NextParameter();
  if (pchParameter == NULL)
    return nDefault;

  return atol(pchParameter);
}

unsigned long CommandParameter::NextParameterAsUnsignedLong( unsigned long nDefault /*= -1*/ )
{
  const char *pchParameter = NextParameter();
  if (pchParameter == NULL)
    return nDefault;

  return strtoul(pchParameter, NULL, 10);
}

double CommandParameter::NextParameterAsDouble( double fDefault /*= 0.0*/ )
{
  const char *pchParameter = NextParameter();
  if (pchParameter == NULL)
    return fDefault;

  return atof(pchParameter);
}
