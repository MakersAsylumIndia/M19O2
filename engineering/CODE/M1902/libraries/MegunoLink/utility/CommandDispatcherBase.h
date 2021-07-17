#pragma once
#include <Arduino.h>
#include "CommandParameter.h"

namespace MLP
{
  struct CommandCallback 
  {
    PGM_P m_strCommand;
    void (*m_Callback)(CommandParameter &rParameters);
  };

  struct VariableMap
  {
    PGM_P m_strName;
    void *m_pVariable;
    uint8_t m_uMaxBufferSize; // for strings. 
    bool(*m_Callback)(VariableMap &rVariableInfo, CommandParameter &rParameters, bool bPrintOnSet);
  };

  class CommandDispatcherBase
  {
    // Array of up to m_uMaxCommands we can match & dispatch. 
    CommandCallback *const m_pCommands;
    const uint8_t m_uMaxCommands;
    uint8_t m_uLastCommand;

    VariableMap *const m_pVariableMap;
    const uint8_t m_uMaxVariables;
    uint8_t m_uLastVariable;

    // Handler called (if not null) when no command matches. 
    void (*m_fnDefaultHandler)();

  protected:
    CommandDispatcherBase(CommandCallback *pCallbackBuffer, uint8_t uCallbackBufferLength, VariableMap *pVariableMapBuffer, uint8_t uVariableMapLength);

  public:
    bool AddCommand(const __FlashStringHelper* pCommand, void(*CallbackFunction)(CommandParameter& rParameters));
    //bool AddCommand(PGM_P pCommand, void(*CallbackFunction)(CommandParameter &rParameters));

    void SetDefaultHandler(void(*CallbackFunction)());
    void ClearCommands();

    bool AddVariable(const __FlashStringHelper *pName, uint8_t &rVariable);
    bool AddVariable(const __FlashStringHelper *pName, uint16_t &rVariable);
    bool AddVariable(const __FlashStringHelper *pName, uint32_t &rVariable);
    bool AddVariable(const __FlashStringHelper *pName, int8_t &rVariable);
    bool AddVariable(const __FlashStringHelper *pName, int16_t &rVariable);
    bool AddVariable(const __FlashStringHelper *pName, int32_t &rVariable);
    bool AddVariable(const __FlashStringHelper *pName, float &rVariable);
    bool AddVariable(const __FlashStringHelper *pName, double &rVariable);
    bool AddVariable(const __FlashStringHelper *pName, char *pchBuffer, uint8_t uMaxBufferSize);
#if defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_SAM) || defined(CORE_TEENSY)
    bool AddVariable(const __FlashStringHelper *pName, int &rVariable);
#endif

    void DispatchCommand(char *pchMessage, Print& rSource) const;

  protected:
    enum EConstants { NO_MATCH = 0 };
    uint8_t MatchCommand(PGM_P pchCommand, const char *pchTest) const;

    static bool ProcessVariable_uint8(VariableMap &rVariableInfo, CommandParameter &rParameters, bool bPrintOnSet);
    static bool ProcessVariable_uint16(VariableMap &rVariableInfo, CommandParameter &rParameters, bool bPrintOnSet);
    static bool ProcessVariable_uint32(VariableMap &rVariableInfo, CommandParameter &rParameters, bool bPrintOnSet);
    static bool ProcessVariable_int8(VariableMap &rVariableInfo, CommandParameter &rParameters, bool bPrintOnSet);
    static bool ProcessVariable_int16(VariableMap &rVariableInfo, CommandParameter &rParameters, bool bPrintOnSet);
    static bool ProcessVariable_int32(VariableMap &rVariableInfo, CommandParameter &rParameters, bool bPrintOnSet);
    static bool ProcessVariable_float(VariableMap &rVariableInfo, CommandParameter &rParameters, bool bPrintOnSet);
    static bool ProcessVariable_double(VariableMap &rVariableInfo, CommandParameter &rParameters, bool bPrintOnSet);
    static bool ProcessVariable_string(VariableMap &rVariableInfo, CommandParameter &rParameters, bool bPrintOnSet);

    bool AddVariable(const __FlashStringHelper *pName, void *pVariable, bool(*ProcessFunction)(VariableMap &rVariableInfo, CommandParameter &rParameters, bool bPrintOnSet));
  };
}