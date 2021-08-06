/* **********************************************************************************************
*  Example program to send data to a MegunoLink Pro table. 
*  Visit http://www.megunolink.com/documentation/table/
*  for more information. 
*  ********************************************************************************************** */

#include "MegunoLink.h" // Helpful functions for communicating with MegunoLink Pro. 

// Millis value when the data was last sent. 
long LastSent;

// Progress value
int Fish, Turtles; 

// The table we are sending data to
Table MyTable;

void setup()
{
  Serial.begin(9600);

  LastSent = millis();
  Fish = 0;
  Turtles = 0;

  MyTable.SetDescription("Fish", "Lives in the sea");
  MyTable.SetDescription("Turtles", "Very slow");
}

void loop()
{
  if ((millis() - LastSent) > 1000)
  {
    LastSent = millis();

    MyTable.SendData("Fish", Fish);
    MyTable.SendData("Turtles", Turtles);
    Fish += 3;
    Turtles += 1;
  }
}