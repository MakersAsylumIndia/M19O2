#include "MegunoLink.h"

XYPlot xyplot("ChannelName");
TimePlot timeplot("ChannelName");
Table table;

float test = 65.987;
void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);

  timeplot.SetTitle("hello");
  timeplot.SetXLabel("test");

}

void loop() 
{
  int DataValue = analogRead(0);

  // put your main code here, to run repeatedly: 
  timeplot.SendData("SeriesName",test,"r-2s2");
  table.SendData("DataValue", DataValue, "this is a reading from analog channel 0");
  xyplot.SendData("SeriesName",millis(),56.589,"b-2s2");
  delay(1000);
}

