/* ************************************************************************
Plot Light Sensor Value
This program demonstrates how MegunoLink's Time Plot visualiser can be used
to plot the analog signal from a light dependent resistor.

The example folder also contains a MegunoLink project with the Time Plot
visulaiser ready to go.
Visit:
* http://www.MegunoLink.com to download MegunoLink.
************************************************************************ */

#include "MegunoLink.h"

void setup() 
{
  Serial.begin(9600);
  Serial.println("Light sensor");
}

void loop() 
{
  TimePlot LightPlot;
  LightPlot.SendData("Light", analogRead(0));
  delay(100);
}
