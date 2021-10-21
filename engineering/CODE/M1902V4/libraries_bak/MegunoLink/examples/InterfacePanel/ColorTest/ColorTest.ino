/************************************************************************************************
Example Description
This example demonstrates Megunolink's Interface Panel and its ability to be updated from commands 
sent from the Arduino. In this example we dynamically adjust the colour of a button on a pre-made
interface.
 
More Information
*  https://www.megunolink.com/documentation/interface-panel/
 
This Example Requires:
*  The MegunoLink arduino library https://www.megunolink.com/documentation/arduino-integration/
 
MegunoLink Interface
You can download a pre-made interface from here:
https://github.com/Megunolink/MLP/raw/master/examples/InterfacePanel/ColorTest/ColorTest.mlpz
 
You can find out more about MegunoLink and download a free trial from here
https://www.megunolink.com/
https://www.megunolink.com/download/
************************************************************************************************/

#include "MegunoLink.h"

void setup() 
{
  Serial.begin(9600);
  Serial.println("Color Test");
}

void loop() 
{
  InterfacePanel UI;

  // Colours can be by name or hex string. 
  // Named colours are here: https://msdn.microsoft.com/en-us/library/system.drawing.color(v=vs.110).aspx
  // Hex values are RRGGBB
  UI.SetBackColor("bttA", "Red");
  UI.SetBackColor("bttB", "00FF00"); // Green

  delay(1000);
  
  UI.SetBackColor("bttA", "AliceBlue");
  UI.SetBackColor("bttB", "FFFF00"); // Yellow

  delay(1000);
}
