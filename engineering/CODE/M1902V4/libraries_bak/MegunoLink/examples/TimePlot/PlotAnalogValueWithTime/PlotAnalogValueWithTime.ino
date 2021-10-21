/* **********************************************************************************************
*  Example program to plot data from the analoge to digital converter on a chart using
*  time supplied from the Aurdio. Normally the Time Plot will use the computer's local time
*  for the x-axis. In this example, we generate our own local time. 
*  Visit http://www.megunolink.com/documentation/plotting/
*  for more information. 
*  ********************************************************************************************** */

#include "MegunoLink.h" 
#include "ArduinoTimer.h"

// The plot we are sending data to. 
TimePlot MyPlot;

// A timer to trigger sending data. 
ArduinoTimer SendDataTimer;

// Timer to maintain the system time. 
ArduinoTimer ClockTick;

/* ----------------------------------------------------------------------------------------------
*  Time Functions
*  Normally keep track of the current time with a real-time clock. Here we fake time
*  using standard system functions. 
 */

void SetupClock()
{
  tm Now;
  memset(&Now, 0, sizeof(Now));
  Now.tm_year = 120; // year relative to 1900. 
  Now.tm_mon = 0;    // Jan = 0, Feb = 1,...
  Now.tm_mday = 4;   // Day of month. 1, 2, ...
  Now.tm_hour = 15;  // Hour of day
  Now.tm_min = 0;    // Minute of hour. 
  Now.tm_sec = 0;    // Second of minute. 

  time_t EpocTime = mktime(&Now);

  set_system_time(EpocTime);
}

void MaintainClock()
{
  if (ClockTick.TimePassed_Milliseconds(1000))
  {
    system_tick();
  }
}

tm GetCurrentTime()
{
  time_t CurrentEpoc = time(NULL);
    Serial.println(CurrentEpoc);
  tm CurrentTime;
  gmtime_r(&CurrentEpoc, &CurrentTime);
  return CurrentTime;
}

/* ----------------------------------------------------------------------------------------------
*  Program setup and loop. 
 */
void setup()
{
  Serial.begin(9600);
  SetupClock();
}

void loop()
{
  MaintainClock();

  if (SendDataTimer.TimePassed_Seconds(1))
  {
    // Using a time structure:
    tm CurrentTime = GetCurrentTime();
    int DataValue0 = analogRead(0);
    MyPlot.SendData("ADCValue0", CurrentTime, DataValue0);

    // Using time epoch. This will be interpreted
    // as local time by MegunoLink. 
    time_t EpochTime = time(NULL) + UNIX_OFFSET;
    float DataValue1 = analogRead(1);
    MyPlot.SendFloatData(F("ADCValue1"), EpochTime, DataValue1, 3);
  }
}
