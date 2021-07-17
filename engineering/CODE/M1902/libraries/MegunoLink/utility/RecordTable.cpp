#include "RecordTable.h"

RecordTable::RecordTable(const char *Channel /*= NULL*/, Print &rDestination /*= Serial*/)
  : MegunoLinkProtocol(F("RECTBL"), Channel, rDestination)
{
  m_uNumberOfDecimalPlaces = 3;
}

RecordTable::RecordTable(const __FlashStringHelper* Channel, Print& rDestination /*= Serial*/)
  : MegunoLinkProtocol(F("RECTBL"), Channel, rDestination)
{
  m_uNumberOfDecimalPlaces = 3;
}

RecordTable::RecordTable(Print& rDestination)
  : MegunoLinkProtocol(F("RECTBL"), (const char*)NULL, rDestination)
{
  m_uNumberOfDecimalPlaces = 3;
}

void RecordTable::ClearAllRows()
{
  SendDataHeader(F("CLEAR"));
  SendDataTail();
}

void RecordTable::ClearRow(int RowNumber)
{
  SendDataHeader(F("CLEAR"));
  m_rDestination.print(RowNumber);
  SendDataTail();
}

void RecordTable::SendHeader_Add()
{
  SendDataHeader(F("ADD"));
}

void RecordTable::SendHeader_AddWithIds()
{
  SendDataHeader(F("ADI"));
}

void RecordTable::SendHeader_Update()
{
  SendDataHeader(F("SET"));
}

void RecordTable::SendHeader_UpdateWithIds()
{
  SendDataHeader(F("STI"));
}

void RecordTable::SendHeader_SetValue()
{
  SendDataHeader(F("STC"));
}

void RecordTable::SendColumnIds(int NumberOfIds, const uint8_t* pColumnIds)
{
  while (NumberOfIds--)
  {
    m_rDestination.print(*pColumnIds++);
    if (NumberOfIds > 0)
    {
      m_rDestination.print(',');
    }
  }
  m_rDestination.print('=');
}