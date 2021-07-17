#include "InterfacePanel.h"

InterfacePanel::InterfacePanel(Print& rDestination)
  : MegunoLinkProtocol(F("UI"), rDestination)
{

}

InterfacePanel::InterfacePanel(const char *channelName, Print &rDestination)
  : MegunoLinkProtocol(F("UI"), channelName, rDestination)
{

}

InterfacePanel::InterfacePanel(const __FlashStringHelper *channelName, Print &rDestination)
  : MegunoLinkProtocol(F("UI"), channelName, rDestination)
{

}

void InterfacePanel::SetText(const __FlashStringHelper *ControlName, float Value, uint8_t DecimalPlaces)
{
  SendTextHeader(ControlName);
  m_rDestination.print(Value, DecimalPlaces);
  SendDataTail();
}

void InterfacePanel::SetText(const char *ControlName, float Value, uint8_t DecimalPlaces)
{
  SendTextHeader(ControlName);
  m_rDestination.print(Value, DecimalPlaces);
  SendDataTail();
}

void InterfacePanel::SetProgress(const char * ControlName, int nValue)
{
  SetNumber(ControlName, (int32_t) nValue);
}

void InterfacePanel::SetGaugeLabel(const char *ControlName, int LabelNumber, const char *Value)
{
  SendGaugeControlHeader(ControlName, LabelNumber);
  m_rDestination.print(Value);
  SendDataTail();
}

void InterfacePanel::SetGaugeLabel(const __FlashStringHelper *ControlName, int LabelNumber, const char *Value)
{
  SendGaugeControlHeader(ControlName, LabelNumber);
  m_rDestination.print(Value);
  SendDataTail();
}

void InterfacePanel::SetGaugeLabel(const __FlashStringHelper *ControlName, int LabelNumber, const __FlashStringHelper *Value)
{
  SendGaugeControlHeader(ControlName, LabelNumber);
  m_rDestination.print(Value);
  SendDataTail();
}

void InterfacePanel::SetNumber(const char * ControlName, float fValue, int nDecimal)
{
  SendControlHeader(ControlName, F("Value"));
  m_rDestination.print(fValue, nDecimal);
  SendDataTail();
}

void InterfacePanel::SetListIndex(const char * ControlName, int nIndex)
{
  SendControlHeader(ControlName, F("SelectedIndex"));
  m_rDestination.print(nIndex);
  SendDataTail();
}

void InterfacePanel::SetListIndex(const __FlashStringHelper * ControlName, int nIndex)
{
  SendControlHeader(ControlName, F("SelectedIndex"));
  m_rDestination.print(nIndex);
  SendDataTail();
}

void InterfacePanel::SetListValue(const char * ControlName, int nValue)
{
  SetNumber(ControlName, (int32_t) nValue);
}

void InterfacePanel::SetListValue(const __FlashStringHelper * ControlName, int nValue)
{
  SetNumber(ControlName, (int32_t) nValue);
}

void InterfacePanel::SetListName(const char * ControlName, const char *ValueName)
{
  SendControlHeader(ControlName, F("SelectedName"));
  m_rDestination.print(ValueName);
  SendDataTail();
}

void InterfacePanel::SetListName(const __FlashStringHelper* ControlName, const __FlashStringHelper* ValueName)
{
  SendControlHeader(ControlName, F("SelectedName"));
  m_rDestination.print(ValueName);
  SendDataTail();
}

void InterfacePanel::SetListName(const char* ControlName, const __FlashStringHelper* ValueName)
{
  SendControlHeader(ControlName, F("SelectedName"));
  m_rDestination.print(ValueName);
  SendDataTail();
}

void InterfacePanel::SetIndicator(const char* ControlName, bool bOn)
{
  SendControlHeader(ControlName, F("On"));
  m_rDestination.print(bOn ? F("True") : F("False"));
  SendDataTail();
}

void InterfacePanel::SetIndicator(const __FlashStringHelper* ControlName, bool bOn)
{
  SendControlHeader(ControlName, F("On"));
  m_rDestination.print(bOn ? F("True") : F("False"));
  SendDataTail();
}

void InterfacePanel::SetCheck(const char * ControlName, bool bChecked)
{
  SendControlHeader(ControlName, F("Checked"));
  m_rDestination.print(bChecked ? F("True") : F("False"));
  SendDataTail();
}

void InterfacePanel::ClearCheck(const char * ControlName)
{
  SendControlHeader(ControlName, F("Checked"));
  m_rDestination.print(F("False"));
  SendDataTail();
}

void InterfacePanel::SetCheck(const char* ControlName, CheckState State)
{
  SendControlHeader(ControlName, F("CheckState"));
  m_rDestination.print((uint8_t)State);
  SendDataTail();
}

void InterfacePanel::SetProgress(const __FlashStringHelper * ControlName, int nValue)
{
  SendControlHeader(ControlName, F("Value"));
  m_rDestination.print((int32_t)nValue);
  SendDataTail();
}

void InterfacePanel::SetNumber(const __FlashStringHelper * ControlName, float fValue, int nDecimal)
{
  SendControlHeader(ControlName, F("Value"));
  m_rDestination.print(fValue, nDecimal);
  SendDataTail();
}

void InterfacePanel::SetCheck(const __FlashStringHelper * ControlName, bool bChecked)
{
  SendControlHeader(ControlName, F("Checked"));
  m_rDestination.print(bChecked ? F("True") : F("False"));
  SendDataTail();
}

void InterfacePanel::SetCheck(const __FlashStringHelper* ControlName, CheckState State)
{
  SendControlHeader(ControlName, F("CheckState"));
  m_rDestination.print((uint8_t)State);
  SendDataTail();
}

void InterfacePanel::ClearCheck(const __FlashStringHelper * ControlName)
{
  SendControlHeader(ControlName, F("Checked"));
  m_rDestination.print(F("False"));
  SendDataTail();
}

void InterfacePanel::CallCommand(const char* CommandName)
{
  SendDataHeader(F("CMD"));
  m_rDestination.print(CommandName);
  SendDataTail();
}

void InterfacePanel::CallCommand(const __FlashStringHelper* CommandName)
{
  SendDataHeader(F("CMD"));
  m_rDestination.print(CommandName);
  SendDataTail();
}

void InterfacePanel::GetValue(const char* ControlName, const char* PropertyName)
{
  SendDataHeader(F("GET"));
  m_rDestination.print(ControlName);
  m_rDestination.print('.');
  m_rDestination.print(PropertyName);
  SendDataTail();
}

void InterfacePanel::GetValue(const __FlashStringHelper* ControlName, const __FlashStringHelper* PropertyName)
{
  SendDataHeader(F("GET"));
  m_rDestination.print(ControlName);
  m_rDestination.print('.');
  m_rDestination.print(PropertyName);
  SendDataTail();
}

void InterfacePanel::SendControlHeader(const char* ControlName, const char* PropertyName)
{
  SendDataHeader(F("SET"));
  m_rDestination.print(ControlName);
  m_rDestination.print('.');
  m_rDestination.print(PropertyName);
  m_rDestination.print('=');
}

void InterfacePanel::SendControlHeader(const char *ControlName, const __FlashStringHelper *PropertyName)
{
  SendDataHeader(F("SET"));
  m_rDestination.print(ControlName);
  m_rDestination.print('.');
  m_rDestination.print(PropertyName);
  m_rDestination.print('=');
}

void InterfacePanel::SendControlHeader(const __FlashStringHelper *ControlName, const __FlashStringHelper *PropertyName)
{
  SendDataHeader(F("SET"));
  m_rDestination.print(ControlName);
  m_rDestination.print('.');
  m_rDestination.print(PropertyName);
  m_rDestination.print('=');
}

void InterfacePanel::SendControlHeader(const __FlashStringHelper* ControlName, const char* PropertyName)
{
  SendDataHeader(F("SET"));
  m_rDestination.print(ControlName);
  m_rDestination.print('.');
  m_rDestination.print(PropertyName);
  m_rDestination.print('=');
}

void InterfacePanel::SendGaugeControlHeader(const char *ControlName, int nLabelNumber)
{
  SendDataHeader(F("SET"));
  m_rDestination.print(ControlName);
  m_rDestination.print('.');
  m_rDestination.print(F("Label"));
  m_rDestination.print(nLabelNumber);
  m_rDestination.print('=');
}

void InterfacePanel::SendGaugeControlHeader(const __FlashStringHelper *ControlName, int nLabelNumber)
{
  SendDataHeader(F("SET"));
  m_rDestination.print(ControlName);
  m_rDestination.print('.');
  m_rDestination.print(F("Label"));
  m_rDestination.print(nLabelNumber);
  m_rDestination.print('=');
}

void InterfacePanel::ShowControl(const char * ControlName, bool bShow)
{
  SendControlHeader(ControlName, F("Visible"));
  PrintBoolean(bShow);
  SendDataTail();
}

void InterfacePanel::ShowControl(const __FlashStringHelper * ControlName, bool bShow)
{
  SendControlHeader(ControlName, F("Visible"));
  PrintBoolean(bShow);
  SendDataTail();
}

void InterfacePanel::HideControl(const char * ControlName)
{
  ShowControl(ControlName, false);
}

void InterfacePanel::HideControl(const __FlashStringHelper * ControlName)
{
  ShowControl(ControlName, false);
}

void InterfacePanel::EnableControl(const char * ControlName, bool bEnable)
{
  SendControlHeader(ControlName, F("Enabled"));
  PrintBoolean(bEnable);
  SendDataTail();
}

void InterfacePanel::EnableControl(const __FlashStringHelper * ControlName, bool bEnable)
{
  SendControlHeader(ControlName, F("Enabled"));
  PrintBoolean(bEnable);
  SendDataTail();
}

void InterfacePanel::DisableControl(const char * ControlName)
{
  EnableControl(ControlName, false);
}

void InterfacePanel::DisableControl(const __FlashStringHelper * ControlName)
{
  EnableControl(ControlName, false);
}

void InterfacePanel::SetForeColor(const char *ControlName, const char *Color)
{
  SendControlHeader(ControlName, F("ForeColor"));
  m_rDestination.print(Color);
  SendDataTail();
}

void InterfacePanel::SetForeColor(const __FlashStringHelper *ControlName, const __FlashStringHelper *Color)
{
  SendControlHeader(ControlName, F("ForeColor"));
  m_rDestination.print(Color);
  SendDataTail();
}

void InterfacePanel::SetBackColor(const char *ControlName, const char *Color)
{
  SendControlHeader(ControlName, F("BackColor"));
  m_rDestination.print(Color);
  SendDataTail();
}

void InterfacePanel::SetBackColor(const __FlashStringHelper *ControlName, const __FlashStringHelper *Color)
{
  SendControlHeader(ControlName, F("BackColor"));
  m_rDestination.print(Color);
  SendDataTail();
}

void InterfacePanel::SetIndicatorColor(const char* ControlName, const char* Color)
{
  SendControlHeader(ControlName, F("IndicatorColor"));
  m_rDestination.print(Color);
  SendDataTail();
}

void InterfacePanel::SetIndicatorColor(const __FlashStringHelper *ControlName, const __FlashStringHelper *Color)
{
  SendControlHeader(ControlName, F("IndicatorColor"));
  m_rDestination.print(Color);
  SendDataTail();
}

void InterfacePanel::SetReadOnly(const char *ControlName, bool ReadOnly)
{
  SendControlHeader(ControlName, F("ReadOnly"));
  m_rDestination.print(ReadOnly ? F("True") : F("False"));
  SendDataTail();
}

void InterfacePanel::SetReadOnly(const __FlashStringHelper *ControlName, bool ReadOnly)
{
  SendControlHeader(ControlName, F("ReadOnly"));
  m_rDestination.print(ReadOnly ? F("True") : F("False"));
  SendDataTail();
}

void InterfacePanel::SetMinimum(const char *ControlName, int Value)
{
  SendControlHeader(ControlName, F("Minimum"));
  m_rDestination.print(Value);
  SendDataTail();
}

void InterfacePanel::SetMaximum(const char *ControlName, int Value)
{
  SendControlHeader(ControlName, F("Maximum"));
  m_rDestination.print(Value);
  SendDataTail();
}

void InterfacePanel::SetMinimum(const __FlashStringHelper *ControlName, int Value)
{
  SendControlHeader(ControlName, F("Minimum"));
  m_rDestination.print(Value);
  SendDataTail();
}

void InterfacePanel::SetMaximum(const __FlashStringHelper *ControlName, int Value)
{
  SendControlHeader(ControlName, F("Maximum"));
  m_rDestination.print(Value);
  SendDataTail();
}

void InterfacePanel::SendValueHeader(const char *ControlName)
{
  SendControlHeader(ControlName, F("Value"));
}

void InterfacePanel::SendValueHeader(const __FlashStringHelper *ControlName)
{
  SendControlHeader(ControlName, F("Value"));
}

void InterfacePanel::SendTextHeader(const char *ControlName)
{
  SendControlHeader(ControlName, F("Text"));
}

void InterfacePanel::SendTextHeader(const __FlashStringHelper *ControlName)
{
  SendControlHeader(ControlName, F("Text"));
}

void InterfacePanel::ShowPrompt(const char* ControlName, int Id /*= 0*/, const char* Prompt /*= nullptr*/)
{
  SendShowPrompt(ControlName, Id, Prompt);
}

void InterfacePanel::ShowPrompt(const __FlashStringHelper* ControlName, int Id /*= 0*/, const char* Prompt /*= nullptr*/)
{
  SendShowPrompt(ControlName, Id, Prompt);
}

void InterfacePanel::ShowPrompt(const __FlashStringHelper* ControlName, int Id, const __FlashStringHelper* Prompt)
{
  SendShowPrompt(ControlName, Id, Prompt);
}

void InterfacePanel::PrintBoolean(bool bValue)
{
  if (bValue)
  {
    m_rDestination.print(F("True"));
  }
  else
  {
    m_rDestination.print(F("False"));
  }
}