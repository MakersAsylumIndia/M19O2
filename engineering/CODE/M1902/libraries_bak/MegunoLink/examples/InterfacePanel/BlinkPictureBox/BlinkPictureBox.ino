/************************************************************************************************
Example Description
This example demonstrates the ability to show and hide pictures in MegunoLink's Interface Panel.
The control of this showing/hiding comes from the Arduino end providing a way to do basic animation.
 
More Information
*  https://www.megunolink.com/documentation/build-arduino-interface/
*  https://www.megunolink.com/documentation/interface-panel/
 
This Example Requires:
*  The MegunoLink arduino library https://www.megunolink.com/documentation/arduino-integration/
 
MegunoLink Interface
You can download a pre-made interface from here:
https://github.com/Megunolink/MLP/raw/master/examples/InterfacePanel/BlinkPictureBox/BlinkPictureBox.mlpz
 
You can find out more about MegunoLink and download a free trial from here
https://www.megunolink.com/
https://www.megunolink.com/download/
************************************************************************************************/


#include "MegunoLink.h"

InterfacePanel Panel;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Panel.ShowControl("pbRed");
  delay(400);
  Panel.HideControl("pbRed");
  delay(450);
  Panel.ShowControl("pbGreen");
  delay(400);
  Panel.HideControl("pbGreen");
  delay(450);
}
