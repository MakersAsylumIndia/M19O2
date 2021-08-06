#include "Table.h"

Table::Table(Print& rDestination /*= Serial*/)
  : MegunoLinkProtocol(F("TABLE"), rDestination)
{

}

Table::Table(const char *Channel, Print &rDestination /*= Serial*/)
  : MegunoLinkProtocol(F("TABLE"), Channel, rDestination)
{

}

Table::Table(const __FlashStringHelper *Channel, Print &rDestination /*= Serial*/)
  : MegunoLinkProtocol(F("TABLE"), Channel, rDestination)
{

}

void Table::SetDescription(const char *RowName, const char *Description)
{
  SendDataHeader(F("DESCRIPTION"));
  m_rDestination.print(RowName);
  m_rDestination.print('|');
  m_rDestination.print(Description);
  SendDataTail();
}

void Table::SetDescription(const __FlashStringHelper *RowName, const char *Description)
{
  SendDataHeader(F("DESCRIPTION"));
  m_rDestination.print(RowName);
  m_rDestination.print('|');
  m_rDestination.print(Description);
  SendDataTail();
}

void Table::SetDescription(const __FlashStringHelper *RowName, const __FlashStringHelper *Description)
{
  SendDataHeader(F("DESCRIPTION"));
  m_rDestination.print(RowName);
  m_rDestination.print('|');
  m_rDestination.print(Description);
  SendDataTail();
}

void Table::ClearAllRows()
{
  SendDataHeader(F("CLEAR"));
  SendDataTail();
}

void Table::ClearRow(const char *Name)
{
  SendDataHeader(F("CLEAR"));
  m_rDestination.print(Name);
  SendDataTail();
}

void Table::ClearRow(const __FlashStringHelper *Name)
{
  SendDataHeader(F("CLEAR"));
  m_rDestination.print(Name);
  SendDataTail();
}

void Table::GetData(const char *Name)
{
  SendDataHeader(F("GET"));
  m_rDestination.print(Name);
  SendDataTail();
}

void Table::GetData(const __FlashStringHelper *Name)
{
  SendDataHeader(F("GET"));
  m_rDestination.print(Name);
  SendDataTail();
}

void Table::SendHeader_Set()
{
  SendDataHeader(F("SET"));
}

void Table::ShowCurrentTime(const char *RowName)
{
  SendHeader_Set();
  m_rDestination.print(RowName);
  m_rDestination.print(F("|[Now()]"));
  SendDataTail();
}

void Table::ShowCurrentTime(const __FlashStringHelper *RowName)
{
  SendHeader_Set();
  m_rDestination.print(RowName);
  m_rDestination.print(F("|[Now()]"));
  SendDataTail();
}

void Table::SendFloatData(const char *RowName, float Value, int DecimalPlaces, const char *Description /* = NULL */)
{
  SendHeader_Set();
  m_rDestination.print(RowName);
  m_rDestination.print('|');
  m_rDestination.print(Value, DecimalPlaces);
  if (Description != NULL)
  {
    m_rDestination.print('|');
    m_rDestination.print(Description);
  }
  SendDataTail();
}

void Table::SendFloatData(const __FlashStringHelper *RowName, float Value, int DecimalPlaces, const char *Description /* = NULL */)
{
  SendHeader_Set();
  m_rDestination.print(RowName);
  m_rDestination.print('|');
  m_rDestination.print(Value, DecimalPlaces);
  if (Description != NULL)
  {
    m_rDestination.print('|');
    m_rDestination.print(Description);
  }
  SendDataTail();
}

void Table::SendFloatData(const __FlashStringHelper *RowName, float Value, int DecimalPlaces, const __FlashStringHelper *Description)
{
  SendHeader_Set();
  m_rDestination.print(RowName);
  m_rDestination.print('|');
  m_rDestination.print(Value, DecimalPlaces);
  if (Description != NULL)
  {
    m_rDestination.print('|');
    m_rDestination.print(Description);
  }
  SendDataTail();
}
