/************************************************************************************************
Example Description
In this example we demonstrate our Filter library. MegunoLink's Interface Panel is used to allow
adjustment of the filter time constant. This allows you to tune the amount of smoothing applied.

More Information
*  http://www.MegunoLink.com/documentation/arduino-libraries/exponential-filter/
*  http://www.megunolink.com/documentation/plotting/

This Example Requires:
*  The MegunoLink arduino library https://www.megunolink.com/documentation/arduino-integration/
 
MegunoLink Interface
You can download a pre-made interface from here:
https://github.com/Megunolink/MLP/raw/master/examples/ExponentialFilterExplorer/ExponentialFilterExplorer.mlpz
 
You can find out more about MegunoLink and download a free trial from here
https://www.megunolink.com/
https://www.megunolink.com/download/
************************************************************************************************/

#include "MegunoLink.h"
#include "Filter.h"
#include "CommandHandler.h"

// Create a new exponential filter with a weight of 10 and initial value of 0. 
ExponentialFilter<long> ADCFilter(10, 0);

CommandHandler<> SerialCommands;

void Cmd_SetFilter(CommandParameter &p)
{
  int NewFilterWeight = p.NextParameterAsInteger(ADCFilter.GetWeight());
  if (NewFilterWeight < 0)
  {
    NewFilterWeight = 0;
  }
  if (NewFilterWeight > 100)
  {
    NewFilterWeight = 100;
  }

  ADCFilter.SetWeight(NewFilterWeight);
  Serial.print(F("Filter weight set to: "));
  Serial.println(NewFilterWeight);
}

void setup() 
{
  Serial.begin(9600);
  Serial.println("Exponential Filter Explorer");
  Serial.println("---------------------------");
  Serial.println();
  Serial.println("Commands: ");
  Serial.println("  !SetFilter value\\r\\n");
  Serial.println("Commands: ");
  Serial.println("  !SetFilter 20\\r\\n set filter parameter to 20%. Range is 0..100");

  SerialCommands.AddCommand(F("SetFilter"), Cmd_SetFilter);
}

void loop() 
{
  SerialCommands.Process();
  
  int RawValue = analogRead(0);
  ADCFilter.Filter(RawValue);

  TimePlot Plot;
  Plot.SendData("Raw", RawValue);
  Plot.SendData("Filtered", ADCFilter.Current());

  delay(100);
}
