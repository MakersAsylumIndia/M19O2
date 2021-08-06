/* **********************************************************************************************
*  Example program to plot data from the analoge to digital converter on an X-Y Plot
*  Visit http://www.megunolink.com/documentation/plotting/
*  for more information. 
*  ********************************************************************************************** */

#include "MegunoLink.h" // Helpful functions for communicating with MegunoLink Pro. 

// Millis value when the data was last sent. 
long LastSent;

// Interval (milliseconds) between sending analog data
const unsigned SendInterval = 200; // [ms]

// The plot we are sending data to. A TimePlot is used here 
XYPlot MyPlot;

void setup()
{
  Serial.begin(9600);

  LastSent = millis();

  MyPlot.SetTitle("My Analog Measurement");
  MyPlot.SetXLabel("Arduino Timer [mS]");
  MyPlot.SetYLabel("Value");
  MyPlot.SetSeriesProperties("ADCValue", Plot::Magenta, Plot::Solid, 2, Plot::Square);
}

void loop()
{
  if ((millis() - LastSent) > SendInterval)
  {
    LastSent = millis();

    int DataValue = analogRead(0);
    MyPlot.SendData("ADCValue", millis(),DataValue); //millis() goes on the xaxis, DataValue goes on the yaxis
  }
}

