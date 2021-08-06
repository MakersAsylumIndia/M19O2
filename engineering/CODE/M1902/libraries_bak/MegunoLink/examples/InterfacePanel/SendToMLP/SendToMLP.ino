/* **********************************************************************************************
*  Example program to set values on a MegunoLink Pro User Interface
*  Panel. Visit http://www.megunolink.com/how-to-update-interface-panel-controls-from-the-arduino/
*  for more information. 
*  
*  This program sends out commands to set the value of a progress bar named 'Progress', a 
*  numeric up/down control named 'Current' and a text box named 'Status' periodically. The 
*  interface panel is included in the MegunoLink Pro project in the example folder. 
*  ********************************************************************************************** */

#include "MegunoLink.h" // Helpful functions for communicating with MegunoLink Pro. 

// Millis value when the data was last sent. 
long LastSent;

// Progress value
int Progress; 

// The interface panel we are sending data to
InterfacePanel Panel;

void setup()
{
  Serial.begin(9600);

  LastSent = millis();
  Progress = 0;
}

void loop()
{
  if ((millis() - LastSent) > 1000)
  {
    LastSent = millis();

    Panel.SetProgress("Progress", Progress);
    Panel.SetNumber("Current", Progress);

    if (Progress == 0)
      Panel.SetText("Status", "Start");
    else if (Progress == 8)
      Panel.SetText("Status", "Nearly there");
    else if (Progress == 9)
      Panel.SetText("Status", "End");
    else
      Panel.SetText("Status", "Working");

    Progress = (Progress + 1) % 10;
  }
}