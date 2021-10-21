#pragma once
#include "MegunoLinkProtocol.h"

enum class SpecialParameters
{
  CurrentTime
};

class RecordTable : public MegunoLinkProtocol
{
  uint8_t m_uNumberOfDecimalPlaces;

public:
  RecordTable(Print& rDestination = Serial);
  RecordTable(const char *Channel, Print &rDestination = Serial);
  RecordTable(const __FlashStringHelper *Channel, Print &rDestination = Serial);

  void SetNumberOfDecimalPlaces(uint8_t uValue)
  {
    m_uNumberOfDecimalPlaces = uValue;
  }

  template<typename... Types> void AddRow(Types... Values)
  {
    SendHeader_Add();
    SendValue(Values...);
    SendDataTail();
  }
  
  template<class TValue> void AddRowFromArray(int NumberOfValues, TValue *pValue)
	{
    SendHeader_Add();
    SendValues(NumberOfValues, pValue);
		SendDataTail();
	}

  template<typename... Types> void AddRowWithIds(int NumberOfIds, const uint8_t *pColumnIds, Types... Values)
  {
    SendHeader_AddWithIds();
    SendColumnIds(NumberOfIds, pColumnIds);
    SendValue(Values...);
    SendDataTail();
  }

  template<typename TValue> void AddRowFromArrayWithIds(int NumberOfIds, const uint8_t* pColumnIds, int NumberOfValues, TValue* pValue)
  {
    SendHeader_AddWithIds();
    SendColumnIds(NumberOfIds, pColumnIds);
    SendValues(NumberOfValues, pValue);
    SendDataTail();
  }

  template<typename... Types> void UpdateRow(int nRow, Types... Values)
  {
    SendHeader_Update();
    m_rDestination.print(nRow);
    m_rDestination.print('|');
    SendValue(Values...);
    SendDataTail();
  }

  template<class TValue> void UpdateRowFromArray(int nRow, int NumberOfValues, TValue* pValue)
  {
    SendHeader_Update();
    m_rDestination.print(nRow);
    m_rDestination.print('|');
    SendValues(NumberOfValues, pValue);
    SendDataTail();
  }

  template<typename... Types> void UpdateRowWithIds(int nRow, int NumberOfIds, const uint8_t* pColumnIds, Types... Values)
  {
    SendHeader_UpdateWithIds();
    m_rDestination.print(nRow);
    m_rDestination.print('|');
    SendColumnIds(NumberOfIds, pColumnIds);
    SendValue(Values...);
    SendDataTail();
  }

  template<typename TValue> void UpdateRowFromArrayWithIds(int nRow, int NumberOfIds, const uint8_t* pColumnIds, int NumberOfValues, TValue* pValue)
  {
    SendHeader_UpdateWithIds();
    m_rDestination.print(nRow);
    m_rDestination.print('|');
    SendColumnIds(NumberOfIds, pColumnIds);
    SendValues(NumberOfValues, pValue);
    SendDataTail();
  }

  template<typename TValue> void SetValue(int nRow, int nColumn, TValue Value)
  {
    SendHeader_SetValue();
    m_rDestination.print(nRow);
    m_rDestination.print('|');
    m_rDestination.print(nColumn);
    m_rDestination.print('|');
    SendValue(Value);
    SendDataTail();
  }

	void ClearAllRows();
	void ClearRow(int RowNumber);

private:
  void SendHeader_Add();
  void SendHeader_AddWithIds();
  void SendHeader_Update();
  void SendHeader_UpdateWithIds();
  void SendHeader_SetValue();
  void SendColumnIds(int NumberOfIds, const uint8_t* pColumnIds);

  template<class TValue> void SendValues(int NumberOfValues, TValue* pValue)
  {
    while (NumberOfValues--)
    {
      SendValue(*pValue++);
      if (NumberOfValues > 0)
      {
        m_rDestination.print(',');
      }
    }
  }

  void SendValue(SpecialParameters Param)
  {
    switch (Param)
    {
    case SpecialParameters::CurrentTime:
      m_rDestination.print(F("[Now()]"));
      break;
    default:
      break;
    }
  }

  void SendValue(float Value)
  {
    m_rDestination.print(Value, m_uNumberOfDecimalPlaces);
  }

  void SendValue(double Value)
  {
    m_rDestination.print(Value, m_uNumberOfDecimalPlaces);
  }

  template<typename T> void SendValue(const T Value)
  {
    m_rDestination.print(Value);
  }

  template<typename T, typename... Types> void SendValue(const T Value1, const Types... Values)
  {
    SendValue(Value1);
    m_rDestination.print(',');
    SendValue(Values...);
  }

};

