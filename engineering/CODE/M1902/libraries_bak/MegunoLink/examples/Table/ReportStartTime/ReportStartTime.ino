/* **********************************************************************************************
*  Example program to send the micro start time to a MegunoLink Pro table.
*  Visit http://www.megunolink.com/documentation/table/ for more information.
*  ********************************************************************************************** */

#include "MegunoLink.h" // Helpful functions for communicating with MegunoLink Pro. 

// The table we are sending data to
Table MyTable;

void setup()
{
  Serial.begin(9600);

  // short delay so the serial port has a chance to initialize
  delay(2000);

  // Tell MegunoLink to display the current time.
  MyTable.ShowCurrentTime("Last Start");
}

void loop()
{
  // do something interesting here
}