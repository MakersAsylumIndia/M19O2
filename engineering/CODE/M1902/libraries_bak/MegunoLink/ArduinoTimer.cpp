#include "ArduinoTimer.h"

ArduinoTimer::ArduinoTimer()
{
  m_uStartTime = 0;
}

bool ArduinoTimer::TimePassed_Milliseconds(uint32_t uPeriod, bool bAutoReset)
{
  uint32_t uNow;

  uNow = millis();
  if (uNow - m_uStartTime >= uPeriod)
  {
    if (bAutoReset)
      m_uStartTime = uNow;
    return true;
  }

  return false;
}

bool ArduinoTimer::TimePassed_Seconds(uint16_t uPeriod, bool bAutoReset)
{
  uint32_t uNow;

  uNow = millis();
  if ((uNow - m_uStartTime)/1000 >= uPeriod)
  {
    if (bAutoReset)
      m_uStartTime = uNow;
    return true;
  }

  return false;
}

bool ArduinoTimer::TimePassed_Minutes(uint16_t uPeriod, bool bAutoReset)
{
  uint32_t uNow;
  uint32_t uDelta;

  uNow = millis();
  uDelta = uNow - m_uStartTime;
  uDelta /= 60000; // => s
  if (uDelta >= uPeriod)
  {
    if (bAutoReset)
      m_uStartTime = uNow;
    return true;
  }

  return false;
}

bool ArduinoTimer::TimePassed_Hours( uint16_t uPeriod, bool bAutoReset /*= true*/ )
{
  uint32_t uNow;
  uint32_t uDelta;

  uNow = millis();
  uDelta = uNow - m_uStartTime;
  uDelta /= 60000; // => s
  uDelta /= 60; // => h
  if (uDelta >= uPeriod)
  {
    if (bAutoReset)
      m_uStartTime = uNow;
    return true;
  }

  return false;
}
