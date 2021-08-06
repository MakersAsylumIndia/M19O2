#include "CommandDispatcherBase.h"

using namespace MLP;


CommandDispatcherBase::CommandDispatcherBase( CommandCallback *pCallbackBuffer, uint8_t uCallbackBufferLength, VariableMap *pVariableMapBuffer, uint8_t uVariableMapLength)
  : m_pCommands(pCallbackBuffer), m_uMaxCommands(uCallbackBufferLength)
  , m_pVariableMap(pVariableMapBuffer), m_uMaxVariables(uVariableMapLength)
{
  m_uLastCommand = 0;
  m_uLastVariable = 0;
  m_fnDefaultHandler = NULL;
}

bool CommandDispatcherBase::AddCommand( const __FlashStringHelper *pCommand, void(*CallbackFunction)(CommandParameter &rParameters) )
{
  if (m_uLastCommand < m_uMaxCommands)
  {
    m_pCommands[m_uLastCommand].m_Callback = CallbackFunction;
    m_pCommands[m_uLastCommand].m_strCommand = (PGM_P )pCommand;
    ++m_uLastCommand;
    return true;
  }

  Serial.println(F("AddCommand: full"));

  return false; // too many commands stored already. 
}

#if 0
bool CommandDispatcherBase::AddCommand(PGM_P pCommand, void(*CallbackFunction)(CommandParameter& rParameters))
{
  if (m_uLastCommand < m_uMaxCommands)
  {
    m_pCommands[m_uLastCommand].m_Callback = CallbackFunction;
    m_pCommands[m_uLastCommand].m_strCommand = pCommand;
    ++m_uLastCommand;
    return true;
  }

  Serial.println(F("AddCommand: full"));

  return false; // too many commands stored already. 
}
#endif



void CommandDispatcherBase::SetDefaultHandler( void(*CallbackFunction)() )
{
  m_fnDefaultHandler = CallbackFunction;
}

void CommandDispatcherBase::ClearCommands()
{
  m_uLastCommand = 0;
}

bool CommandDispatcherBase::AddVariable(const __FlashStringHelper *pName, uint8_t &rVariable)
{
  return AddVariable(pName, &rVariable, ProcessVariable_uint8);
}

bool CommandDispatcherBase::AddVariable(const __FlashStringHelper *pName, uint16_t &rVariable)
{
  return AddVariable(pName, &rVariable, ProcessVariable_uint16);
}

bool CommandDispatcherBase::AddVariable(const __FlashStringHelper *pName, uint32_t &rVariable)
{
  return AddVariable(pName, &rVariable, ProcessVariable_uint32);
}

bool CommandDispatcherBase::AddVariable(const __FlashStringHelper *pName, int8_t &rVariable)
{
  return AddVariable(pName, &rVariable, ProcessVariable_int8);
}

bool CommandDispatcherBase::AddVariable(const __FlashStringHelper *pName, int16_t &rVariable)
{
  return AddVariable(pName, &rVariable, ProcessVariable_int16);
}

bool CommandDispatcherBase::AddVariable(const __FlashStringHelper *pName, int32_t &rVariable)
{
  return AddVariable(pName, &rVariable, ProcessVariable_int32);
}

#if defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_SAM) || defined(CORE_TEENSY)
bool CommandDispatcherBase::AddVariable(const __FlashStringHelper *pName, int &rVariable)
{
  return AddVariable(pName, &rVariable, ProcessVariable_int32);
}
#endif

bool CommandDispatcherBase::AddVariable(const __FlashStringHelper *pName, float &rVariable)
{
  return AddVariable(pName, &rVariable, ProcessVariable_float);
}

bool CommandDispatcherBase::AddVariable(const __FlashStringHelper *pName, double &rVariable)
{
  return AddVariable(pName, &rVariable, ProcessVariable_double);
}

bool CommandDispatcherBase::AddVariable(const __FlashStringHelper *pName, char *pchBuffer, uint8_t uMaxBufferSize)
{
  if (m_uLastVariable < m_uMaxVariables)
  {
    m_pVariableMap[m_uLastVariable].m_strName = (PGM_P )pName;
    m_pVariableMap[m_uLastVariable].m_pVariable = pchBuffer;
    m_pVariableMap[m_uLastVariable].m_uMaxBufferSize = uMaxBufferSize;
    m_pVariableMap[m_uLastVariable].m_Callback = ProcessVariable_string;
    ++m_uLastVariable;
    return true;
  }

  Serial.println(F("AddVariable: full"));
  return false; // too many variables stored already. 
}

bool CommandDispatcherBase::AddVariable(const __FlashStringHelper *pName, void *pVariable, bool(*ProcessFunction)(VariableMap &rVariableInfo, CommandParameter &rParameters, bool bPrintOnSet))
{
  if (m_uLastVariable < m_uMaxVariables)
  {
    m_pVariableMap[m_uLastVariable].m_strName = (PGM_P )pName;
    m_pVariableMap[m_uLastVariable].m_pVariable = pVariable;
    m_pVariableMap[m_uLastVariable].m_Callback = ProcessFunction;
    ++m_uLastVariable;
    return true;
  }

  Serial.println(F("AddVariable: full"));
  return false; // too many variables stored already. 
}

void CommandDispatcherBase::DispatchCommand( char *pchMessage, Print &rSource ) const
{
  uint8_t uCommand, uParameterStart;
  CommandCallback *pCommand;
  uCommand = m_uLastCommand;
  pCommand = m_pCommands;
  while(uCommand--)
  {
    uParameterStart = MatchCommand(pCommand->m_strCommand, pchMessage);

    if (uParameterStart != NO_MATCH)
    {
      CommandParameter Parameters(rSource, pchMessage, uParameterStart);
      pCommand->m_Callback(Parameters);
      return;
    }

    ++pCommand;
  }

  uCommand = m_uLastVariable;
  VariableMap *pVariableMap = m_pVariableMap;
  while (uCommand--)
  {
    uParameterStart = MatchCommand(pVariableMap->m_strName, pchMessage);
    if (uParameterStart != NO_MATCH)
    {
      CommandParameter Parameters(rSource, pchMessage, uParameterStart);
      pVariableMap->m_Callback(*pVariableMap, Parameters, true);
      return;
    }

    ++pVariableMap;
  }

  // No command matched. 
  if (m_fnDefaultHandler != NULL)
  {
    (*m_fnDefaultHandler)();
  }
  else
  {
    Serial.println(F("Unknown command"));
  }
}

uint8_t CommandDispatcherBase::MatchCommand( PGM_P pchCommand, const char *pchTest ) const
{
  char chCommand, chTest;
  uint8_t uParameterStart = 0;
  do 
  {
    chCommand = pgm_read_byte_near(pchCommand++);
    chTest = *pchTest++;
    ++uParameterStart;
  } while (chCommand == chTest && chCommand != '\0' && (chTest != ' ' || chTest != '\0'));

  if (chCommand == '\0' && (chTest == ' ' || chTest == '\0'))
    return uParameterStart;

  return NO_MATCH; // No match. 
}

Print & PrintVariableName(VariableMap &rVariableInfo, CommandParameter &rParameters)
{
  Print &rOut = rParameters.GetSource();
  rOut.print(reinterpret_cast<const __FlashStringHelper*>(rVariableInfo.m_strName));
  rOut.print('=');

  return rOut;
}

template<class T> bool ProcessIntVariable(VariableMap &rVariableInfo, CommandParameter &rParameters, bool bPrintOnSet)
{
  const char *pchParam = rParameters.RemainingParameters();
  T *pValue = (T*)(rVariableInfo.m_pVariable);

  if (*pchParam != '?')
  {
    *pValue = (T)rParameters.NextParameterAsInteger(*pValue);
  }

  if (*pchParam == '?' || bPrintOnSet)
  {
    PrintVariableName(rVariableInfo, rParameters).println(*pValue);
  }

  return true;
}

bool CommandDispatcherBase::ProcessVariable_uint8(VariableMap &rVariableInfo, CommandParameter &rParameters, bool bPrintOnSet)
{
  return ProcessIntVariable<uint8_t>(rVariableInfo, rParameters, bPrintOnSet);
}

bool CommandDispatcherBase::ProcessVariable_uint16(VariableMap &rVariableInfo, CommandParameter &rParameters, bool bPrintOnSet)
{
  return ProcessIntVariable<uint16_t>(rVariableInfo, rParameters, bPrintOnSet);
}

bool CommandDispatcherBase::ProcessVariable_uint32(VariableMap &rVariableInfo, CommandParameter &rParameters, bool bPrintOnSet)
{
  const char *pchParam = rParameters.RemainingParameters();
  uint32_t *pValue = (uint32_t*)(rVariableInfo.m_pVariable);

  if (*pchParam != '?')
  {
    *pValue = (uint32_t)rParameters.NextParameterAsUnsignedLong(*pValue);
  }

  if (*pchParam == '?' || bPrintOnSet)
  {
    PrintVariableName(rVariableInfo, rParameters).println(*pValue);
  }

  return true;
}

bool CommandDispatcherBase::ProcessVariable_int8(VariableMap &rVariableInfo, CommandParameter &rParameters, bool bPrintOnSet)
{
  return ProcessIntVariable<int8_t>(rVariableInfo, rParameters, bPrintOnSet);
}

bool CommandDispatcherBase::ProcessVariable_int16(VariableMap &rVariableInfo, CommandParameter &rParameters, bool bPrintOnSet)
{
  return ProcessIntVariable<int16_t>(rVariableInfo, rParameters, bPrintOnSet);
}

bool CommandDispatcherBase::ProcessVariable_int32(VariableMap &rVariableInfo, CommandParameter &rParameters, bool bPrintOnSet)
{
  const char *pchParam = rParameters.RemainingParameters();
  int32_t *pValue = (int32_t*)(rVariableInfo.m_pVariable);

  if (*pchParam != '?')
  {
    *pValue = rParameters.NextParameterAsLong(*pValue);
  }

  if (*pchParam == '?' || bPrintOnSet)
  {
    PrintVariableName(rVariableInfo, rParameters).println(*pValue);
  }

  return true;
}

bool CommandDispatcherBase::ProcessVariable_float(VariableMap &rVariableInfo, CommandParameter &rParameters, bool bPrintOnSet)
{
  const char *pchParam = rParameters.RemainingParameters();
  float *pValue = (float*)(rVariableInfo.m_pVariable);

  if (*pchParam != '?')
  {
    *pValue = (float)rParameters.NextParameterAsDouble(*pValue);
  }

  if (*pchParam == '?' || bPrintOnSet)
  {
    PrintVariableName(rVariableInfo, rParameters).println(*pValue);
  }

  return true;
}

bool CommandDispatcherBase::ProcessVariable_double(VariableMap &rVariableInfo, CommandParameter &rParameters, bool bPrintOnSet)
{
  const char *pchParam = rParameters.RemainingParameters();
  double *pValue = (double*)(rVariableInfo.m_pVariable);

  if (*pchParam != '?')
  {
    *pValue = rParameters.NextParameterAsDouble(*pValue);
  }

  if (*pchParam == '?' || bPrintOnSet)
  {
    PrintVariableName(rVariableInfo, rParameters).println(*pValue);
  }

  return true;
}

bool CommandDispatcherBase::ProcessVariable_string(VariableMap &rVariableInfo, CommandParameter &rParameters, bool bPrintOnSet)
{
  const char *pchParam = rParameters.RemainingParameters();
  char *pValue = (char*)rVariableInfo.m_pVariable;

  if (*pchParam != '?')
  {
    uint8_t uMaxLength = rVariableInfo.m_uMaxBufferSize - 1;
    const char *pchSource = pchParam;
    char *pchDestination = pValue;
    while (uMaxLength-- && *pchSource)
    {
      *pchDestination++ = *pchSource++;
    }
    *pchDestination = '\0';
  }

  if (*pchParam == '?' || bPrintOnSet)
  {
    PrintVariableName(rVariableInfo, rParameters).println(pValue);
  }

  return true;
}
