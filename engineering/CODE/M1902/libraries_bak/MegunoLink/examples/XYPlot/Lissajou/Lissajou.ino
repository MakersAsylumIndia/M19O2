/* **********************************************************************************************
*  Example program to plot a Lissajous curve on MegunoLink's X-Y Plot visualiser
*  Visit http://www.megunolink.com/documentation/plotting/
*  for more information. 
*  ********************************************************************************************** */

#include "MegunoLink.h"

float t;
double i=0;
Plot::Colors PlotColors[] = {Plot::Red, Plot::Blue, Plot::Green};
const int NumberOfLineStyles = sizeof(PlotColors)/sizeof(PlotColors[0]);
byte ColourIndex = 0; //Array index selecting current colour
long LastColourChangeTime;

XYPlot MyPlot("Lissajou");

void setup()
{
  Serial.begin(9600);

  //Send Plot Title And Axis Labels To MegunoLink Pro
  MyPlot.SetTitle("Lissajou Plot");
  MyPlot.SetXLabel("Xaxis Value");
  MyPlot.SetYLabel("Yaxis Value");

  // Set the plotting parameters. "Sinewave" = series name, Plot::Blue = line colour
  // 2 = line width, Plot::Square = marker style
  MyPlot.SetSeriesProperties("Lissajou", Plot::Blue, Plot::Solid, 2, Plot::Square);

  // Colours include
  // Red, Green, Blue, Yellow, Black, Magenta, Cyan, White
  
  // Markers include
  // Square, Diamond, Triangle, Circle, Cross, Plus, Star, DownwardTriangle, NoMarker
  
  // Line style
  // Solid, Dashed, Dotted, DashDot, DashDotDot

  t = 0;
}

void loop()
{
  float x, y;

  x = sin(5 * t + 1.57);
  y = cos(4 * t);
  t = t + 0.03;

  //Send Plotting Data To MegunoLink Pro
  MyPlot.SendData(F("Lissajou"),x,y); // Lissajou = series name, x = xaxis value, y = yaxis value
  MyPlot.SetSeriesProperties("Lissajou", PlotColors[ColourIndex], Plot::Solid, 2, Plot::Square);
  
  if((millis()-LastColourChangeTime)>1500) //Every 1500ms change the colour
  {
    ++ColourIndex;
    if(ColourIndex >= NumberOfLineStyles)
      ColourIndex = 0;
      
    LastColourChangeTime = millis();
  }
  
  
  
  delay(200);

}

