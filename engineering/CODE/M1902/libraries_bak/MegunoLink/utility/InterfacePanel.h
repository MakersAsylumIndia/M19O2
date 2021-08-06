#pragma once
#include <Arduino.h>
#include "MegunoLinkProtocol.h"

class InterfacePanel : public MegunoLinkProtocol
{
public:
  enum CheckState
  {
    Unchecked = 0,

    Checked = 1,

    Indeterminate = 2,
  };

public:
  InterfacePanel(const char *channelName, Print &rDestination = Serial);
  InterfacePanel(const __FlashStringHelper* channelName, Print& rDestination = Serial);
  InterfacePanel(Print &rDestination = Serial);

  void SetGaugeLabel(const char *ControlName, int LabelNumber, const char *Value);
  void SetGaugeLabel(const __FlashStringHelper *ControlName, int LabelNumber, const char *Value);
  void SetGaugeLabel(const __FlashStringHelper *ControlName, int LabelNumber, const __FlashStringHelper *Value);

  void SetProgress(const char * ControlName, int nValue);
  void SetProgress(const __FlashStringHelper * ControlName, int nValue);

  template<class T>
  void SetText(const char *ControlName, T Value)
  {
    SendTextHeader(ControlName);
    m_rDestination.print(Value);
    SendDataTail();
  }

  template<class T>
  void SetText(const __FlashStringHelper *ControlName, T Value)
  {
    SendTextHeader(ControlName);
    m_rDestination.print(Value);
    SendDataTail();
  }

  void SetText(const __FlashStringHelper *ControlName, float Value, uint8_t DecimalPlaces);
  void SetText(const char *ControlName, float Value, uint8_t DecimalPlaces);

  template<class T>
  void SetNumber(const char *ControlName, T Value)
  {
    SendValueHeader(ControlName);
    m_rDestination.print(Value);
    SendDataTail();
  }

  template<class T>
  void SetNumber(const __FlashStringHelper *ControlName, T Value)
  {
    SendValueHeader(ControlName);
    m_rDestination.print(Value);
    SendDataTail();
  }

  void SetNumber(const char * ControlName, float fValue, int decimal = 3);
  void SetNumber(const __FlashStringHelper * ControlName, float fValue, int decimal = 3);

  void SetListIndex(const char * ControlName, int nIndex);
  void SetListIndex(const __FlashStringHelper * ControlName, int nIndex);
  void SetListValue(const char * ControlName, int nIndex);
  void SetListValue(const __FlashStringHelper * ControlName, int nIndex);
  void SetListName(const char * ControlName, const char *ValueName);
  void SetListName(const __FlashStringHelper* ControlName, const __FlashStringHelper* ValueName);
  void SetListName(const char* ControlName, const __FlashStringHelper *ValueName);
  

  template<class TControl> void ClearListOptions(TControl ControlName)
  {
    SendDataHeader(F("CALL"));
    m_rDestination.print(ControlName);
    m_rDestination.print(F(".ClearListOptions()"));
    SendDataTail();
  }

  template<class TControl, class TDisplayValue> void SetListOption(TControl ControlName, unsigned uValue, const TDisplayValue DisplayValue)
  {
    SendDataHeader(F("CALL"));
    m_rDestination.print(ControlName);
    m_rDestination.print(F(".SetListOption("));
    m_rDestination.print(uValue);
    m_rDestination.print(F(", \""));
    m_rDestination.print(DisplayValue);
    m_rDestination.print(F("\")"));
    SendDataTail();
  }

  void SetIndicator(const char* ControlName, bool bChecked = true);
  void SetIndicator(const __FlashStringHelper* ControlName, bool bChecked = true);

  void SetCheck(const char * ControlName, bool bChecked = true);
  void SetCheck(const __FlashStringHelper * ControlName, bool bChecked = true);
  void SetCheck(const char* ControlName, CheckState State);
  void SetCheck(const __FlashStringHelper* ControlName, CheckState State);

  void ShowControl(const char * ControlName, bool bShow = true);
  void HideControl(const char * ControlName);

  void ShowControl(const __FlashStringHelper * ControlName, bool bShow = true);
  void HideControl(const __FlashStringHelper * ControlName);

  void EnableControl(const char * ControlName, bool bShow = true);
  void DisableControl(const char * ControlName);

  void EnableControl(const __FlashStringHelper * ControlName, bool bShow = true);
  void DisableControl(const __FlashStringHelper * ControlName);

  void SetForeColor(const char *ControlName, const char *Color);
  void SetForeColor(const __FlashStringHelper *ControlName, const __FlashStringHelper *Color);

  void SetBackColor(const char *ControlName, const char *Color);
  void SetBackColor(const __FlashStringHelper *ControlName, const __FlashStringHelper *Color);

  void SetIndicatorColor(const char* ControlName, const char* Color);
  void SetIndicatorColor(const __FlashStringHelper* ControlName, const __FlashStringHelper* Color);

  void ClearCheck(const char * ControlName);
  void ClearCheck(const __FlashStringHelper * ControlName);

  void CallCommand(const char* CommandName);
  void CallCommand(const __FlashStringHelper* CommandName);

  void GetValue(const char* ControlName, const char* PropertyName);
  void GetValue(const __FlashStringHelper* ControlName, const __FlashStringHelper* PropertyName);

  void SetReadOnly(const char *ControlName, bool ReadOnly);
  void SetReadOnly(const __FlashStringHelper *ControlName, bool ReadOnly);

  void SetMinimum(const char *ControlName, int Value);
  void SetMaximum(const char *ControlName, int Value);

  void SetMinimum(const __FlashStringHelper *ControlName, int Value);
  void SetMaximum(const __FlashStringHelper *ControlName, int Value);

  void ShowPrompt(const char* ControlName, int Id = 0, const char* Prompt = nullptr);
  void ShowPrompt(const __FlashStringHelper* ControlName, int Id = 0, const char* Prompt = nullptr);
  void ShowPrompt(const __FlashStringHelper* ControlName, int Id, const __FlashStringHelper* Prompt);

  template<class TControl> void StartProgram(TControl ControlName)
  {
    SendDataHeader(F("CALL"));
    m_rDestination.print(ControlName);
    m_rDestination.print(F(".Run()"));
    SendDataTail();
  }

  template<class TControl, class TArg> void StartProgram(TControl ControlName, TArg Arguments)
  {
    SendDataHeader(F("CALL"));
    m_rDestination.print(ControlName);
    m_rDestination.print(F(".Run(\""));
    m_rDestination.print(Arguments);
    m_rDestination.print(F("\")"));
    SendDataTail();
  }

  template<class TControl> void SetTab(TControl ControlName, int Tab)
  {
    SendControlHeader(ControlName, F("ActiveTab"));
    m_rDestination.print(Tab);
    SendDataTail();
  }

protected:
  void SendControlHeader(const char* ControlName, const char* PropertyName);
  void SendControlHeader(const char *ControlName, const __FlashStringHelper *PropertyName);
  void SendControlHeader(const __FlashStringHelper* ControlName, const __FlashStringHelper* PropertyName);
  void SendControlHeader(const __FlashStringHelper *ControlName, const char*PropertyName);

  void SendGaugeControlHeader(const char *ControlName, int nLabelNumber);
  void SendGaugeControlHeader(const __FlashStringHelper *ControlName, int nLabelNumber);

  void SendValueHeader(const char *ControlName);
  void SendValueHeader(const __FlashStringHelper *ControlName);
  void SendTextHeader(const char *ControlName);
  void SendTextHeader(const __FlashStringHelper *ControlName);


private:
  template <typename TControlName, typename TPrompt> void SendShowPrompt(const TControlName* ControlName, int nId, const TPrompt* Prompt)
  {
    SendDataHeader(F("CALL"));
    m_rDestination.print(ControlName);
    m_rDestination.print('.');
    m_rDestination.print(F("Prompt("));
    m_rDestination.print(nId);
    if (Prompt != nullptr)
    {
      m_rDestination.print(F(",\""));
      m_rDestination.print(Prompt);
      m_rDestination.print('"');
    }
    m_rDestination.println(F(")}"));
  }

  void PrintBoolean(bool bValue);
};
