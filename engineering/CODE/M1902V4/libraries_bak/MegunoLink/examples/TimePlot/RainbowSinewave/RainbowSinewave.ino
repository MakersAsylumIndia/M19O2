/************************************************************************************************
Example Description
In this example we use MegunoLink's Time Plot visualizer to plot Sinewave data in real time. The
plot color is changed from red -> green -> blue. 
 
More Information
*  http://www.megunolink.com/documentation/plotting/

This Example Requires:
*  The MegunoLink arduino library https://www.megunolink.com/documentation/arduino-integration/

MegunoLink Interface
You can download a pre-made interface from here:
https://github.com/Megunolink/MLP/raw/master/examples/TimePlot/Sinewave/Sinewave.mlpz
 
You can find out more about MegunoLink and download a free trial from here
https://www.megunolink.com/
https://www.megunolink.com/download/
************************************************************************************************/

#include "MegunoLink.h"

// Uncomment if you would like to use plotting channels
// TimePlot MyPlot("Waveforms"); //"Waveforms" = the taget plotting channel (remember to select this in megunolink)

TimePlot MyPlot; //no channel selected

uint32_t Colors[3] = {0, 0, 0};
int ColorIndex = 0;

void setup()
{
  Serial.begin(9600);

  MyPlot.SetTitle("Sine and Cosine Function Waveforms");
  MyPlot.SetXLabel("Time");
  MyPlot.SetYLabel("Amplitude");
  
  // Set the plotting parameters. "Sinewave" = series name, Plot::Blue = line colour
  // 2 = line width, Plot::Square = marker style
  MyPlot.SetSeriesProperties("Sinewave", Plot::Blue, Plot::Solid, 2, Plot::Square);
  MyPlot.SetSeriesProperties("Cosinewave", Plot::Red, Plot::Solid, 2, Plot::Square);

  // Colours include
  // Red, Green, Blue, Yellow, Black, Magenta, Cyan, White
  
  // Markers include
  // Square, Diamond, Triangle, Circle, Cross, Plus, Star, DownwardTriangle, NoMarker
  
  // Line style
  // Solid, Dashed, Dotted, DashDot, DashDotDot

}


void loop()
{
  double dY, dY2;
  float seconds;
  float frequency = 0.5; //Hz
  float phase = 3.141/2;

  seconds = (float)millis()/1000;

  dY = sin(2 * 3.141 * frequency * seconds);
  dY2 = cos(2 * 3.141 * frequency * seconds + phase);

  uint32_t Color1 = (Colors[0] << 16) | (Colors[1] << 8) | Colors[2];
  uint32_t Color2 = (Colors[1] << 16) | (Colors[2] << 8) | Colors[0];

  Colors[ColorIndex] = (Colors[ColorIndex] + 4) & 0xff;
  if (Colors[ColorIndex] == 0)
  {
    ColorIndex = (ColorIndex + 1) % 3;
  }
  
  //Send Data To MegunoLink Pro
  MyPlot.SendData(F("Sinewave"),dY, Color1); // Sinewave = series name, dY = data to plot
  MyPlot.SendData(F("Cosinewave"),dY2, Color2); // By wrapping strings in F("") we can save ram by storing strings in program memory


  delay(10);
}
