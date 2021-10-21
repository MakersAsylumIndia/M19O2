/* *****************************************************************************
*  A wraper to simplify timed actions on the Arduino platform. 
*  Usage:
*     // Create a class or global timer variable. 
*     ArduinoTimer MyTimer; 
*     
*     // For timed action.
*     if (MyTimer.TimePassed_Seconds(10))
*     {
*       Serial.println("Another 10 seconds has ellapsed");
*     }
*  ***************************************************************************** */

#pragma once
#include <Arduino.h>

class ArduinoTimer
{
  uint32_t m_uStartTime;
public:
  ArduinoTimer();
  
  inline ArduinoTimer(bool bReset)  __attribute__((always_inline))
  {
    if (bReset)
      Reset();
  }

  inline void Reset() __attribute__((always_inline))
  {
    m_uStartTime = millis();
  }

  inline uint32_t EllapsedMilliseconds() const __attribute__((always_inline))
  {
    return millis() - m_uStartTime;
  }

  inline uint16_t EllapsedSeconds() const __attribute__((always_inline))
  {
    return (millis() - m_uStartTime) / 1000;
  }

  bool TimePassed_Milliseconds(uint32_t uPeriod, bool bAutoReset = true);
  bool TimePassed_Seconds(uint16_t uPeriod, bool bAutoReset = true);
  bool TimePassed_Minutes(uint16_t uPeriod, bool bAutoReset = true);
  bool TimePassed_Hours(uint16_t uPeriod, bool bAutoReset = true);

  inline uint32_t StartTime() const __attribute__((always_inline))
  {
    return m_uStartTime; 
  }
};
