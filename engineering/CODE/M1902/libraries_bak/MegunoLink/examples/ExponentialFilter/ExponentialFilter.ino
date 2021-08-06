/************************************************************************************************
Example Description
In this example we demonstrate our Filter library. We sample data from an analog port then filter
the data and plot both value to MegunoLink's Time Plot visualier.
 
More Information
*  http://www.MegunoLink.com/documentation/arduino-libraries/exponential-filter/
*  http://www.megunolink.com/documentation/plotting/

This Example Requires:
*  The MegunoLink arduino library https://www.megunolink.com/documentation/arduino-integration/
 
MegunoLink Interface
You can download a pre-made interface from here:
https://github.com/Megunolink/MLP/raw/master/examples/ExponentialFilter/ExponentialFilter.mlpz
 
You can find out more about MegunoLink and download a free trial from here
https://www.megunolink.com/
https://www.megunolink.com/download/
************************************************************************************************/

#include "MegunoLink.h"
#include "Filter.h"

// Create a new exponential filter with a weight of 10 and initial value of 0. 
ExponentialFilter<long> ADCFilter(10, 0);

void setup() 
{
  Serial.begin(9600);
  Serial.println("Exponential Filter demo");
  Serial.println("-----------------------");
}

void loop() 
{
  int RawValue = analogRead(0);
  ADCFilter.Filter(RawValue);

  TimePlot Plot;
  Plot.SendData("Raw", RawValue);
  Plot.SendData("Filtered", ADCFilter.Current());

  delay(100);
}
