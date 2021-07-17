/* **********************************************************************************************
*  Example program to send data to a MegunoLink property table and
*  receive property values back that update variables in the sketch. 
*  Visit http://www.megunolink.com/documentation/property-table/
*  for more information. 
*  ********************************************************************************************** */

#include "MegunoLink.h"     // for communicating with MegunoLink. 
#include "ArduinoTimer.h"   // a timer to send data.
#include "CommandHandler.h" // for handling serial commands. 

// Timer to keep track of sending data to MegunoLink.
ArduinoTimer SendTimer;

// Interval between sending updates to MegunoLink
long UpdateInterval = 1000; // milliseconds. 

// Values we keep track of
int Fish = 0;
int Turtles = 0; 

// The table we are sending data to
Table MyTable;

// Command handlers to receive properties from MegunoLink
CommandHandler<> SerialCommands;

void setup()
{
  Serial.begin(9600);
  Serial.println(F("Send data to MegunoLink table"));

  // Link variables to names we'll receive from MegunoLink
  // when the user sends properties from the table. See
  // https://www.megunolink.com/documentation/getting-started/processing-serial-commands/
  SerialCommands.AddVariable(F("Fish"), Fish);
  SerialCommands.AddVariable(F("Turtles"), Turtles);
  SerialCommands.AddVariable(F("UpdateInterval"), UpdateInterval);

  // Set descriptions for the properties that we'll send
  // the the MegunoLink table. 
  MyTable.SetDescription("Fish", "Lives in the sea");
  MyTable.SetDescription("Turtles", "Very slow");
}

void loop()
{
  // Receive and process serial commands.
  SerialCommands.Process();
  
  // Periodically send information to MegunoLink. See
  // https://www.megunolink.com/documentation/arduino-libraries/arduino-timer/
  if (SendTimer.TimePassed_Milliseconds(UpdateInterval))
  {
    MyTable.SendData("Fish", Fish);
    MyTable.SendData("Turtles", Turtles);
    Fish += 3;
    Turtles += 1;
  }
}
