// Only supported for AVR micros because we use the special EEMEM directive
// to automatically allocated memory in the eeprom. 
#if defined(__AVR__)

#include <avr/eeprom.h>
#include <util/crc16.h>

template <class TData> class EEPROMStore
{
  // The data stored in the eprom. The EEMEM complier attribute instructs
  // the complier to locate this variable in the eeprom. 
  struct EEMEM CEEPROMData
  {
    uint16_t m_uChecksum;
    TData m_UserData;

  } m_EEPROMData;

public:
  TData Data;

  EEPROMStore()
  {
    Reset();
    if (!Load())
      Reset();
  }

  bool Load()
  {
    CEEPROMData WorkingCopy;
    if (Load(WorkingCopy))
    {
      memcpy(&Data, &WorkingCopy.m_UserData, sizeof(TData));
      return true;
    }

    return false;
  }

  bool Save()
  {
    // We only save if the current version in the eeprom doesn't match the data we plan to save. 
    // This helps protect the eeprom against save called many times within the arduino loop,
    // though it makes things a little slower. 
    uint16_t uChecksum = CalculateChecksum(Data);
    CEEPROMData StoredVersion;
    if (!Load(StoredVersion) || StoredVersion.m_uChecksum != uChecksum || memcmp(&StoredVersion.m_UserData, &Data, sizeof(Data)) != 0)
    {
      eeprom_write_word(&m_EEPROMData.m_uChecksum, uChecksum);
      eeprom_write_block(&Data, &m_EEPROMData.m_UserData, sizeof(Data));
      return true; 
    }
    return false; 
  }

  void Reset()
  {
    Data.Reset();
  }

private:
  bool Load(CEEPROMData &Result)
  {
    eeprom_read_block(&Result, (const void *)&m_EEPROMData, sizeof(CEEPROMData));
    uint16_t uChecksum = CalculateChecksum(Result.m_UserData);
    return uChecksum == Result.m_uChecksum;
  }

  uint16_t CalculateChecksum(const TData &TestData) const
  {
    uint16_t uChecksum = 0;
    const uint8_t *pRawData = reinterpret_cast<const uint8_t *>(&TestData);
    size_t szData = sizeof(TestData);

    while (szData--)
    {
      uChecksum = _crc16_update(uChecksum, *pRawData++);
    }

    return uChecksum;
  }
};
#else
#error EEPROMStore is only supported on AVR micros.
#endif
