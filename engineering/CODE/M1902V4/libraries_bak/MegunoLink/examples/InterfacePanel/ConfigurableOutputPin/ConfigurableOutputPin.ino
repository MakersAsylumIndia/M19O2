/************************************************************************************************
Example Description
This program demonstrates how MegunoLink's Interface Panel and our command
handler Arduino library can be used to reconfigure which IO pin your device
is using at runtime.
 
More Information
*  https://www.megunolink.com/documentation/interface-panel/
 
This Example Requires:
*  The MegunoLink arduino library https://www.megunolink.com/documentation/arduino-integration/
 
MegunoLink Interface
You can download a pre-made interface from here:
https://github.com/Megunolink/MLP/raw/master/examples/InterfacePanel/ConfigurableOutputPin/ConfigurableOutputPin.mlpz
 
You can find out more about MegunoLink and download a free trial from here
https://www.megunolink.com/
https://www.megunolink.com/download/
************************************************************************************************/

#include "MegunoLink.h"
#include "CommandHandler.h"


int IOPort = 9; //initial IO pin number

long LastToggleTime = 0; // Time we last toggled the IO
int OnTime = 10; // Amount of time the IO remains on [milliseconds]
int OffTime = 100; // Amount of time the IO remains off [milliseconds]

CommandHandler<> SerialCommandHandler;

void Cmd_SetIOPin(CommandParameter &Parameters)
{
  Serial.print("Setting IO Port to:");
  IOPort = Parameters.NextParameterAsInteger();
  pinMode(IOPort, OUTPUT);
  Serial.println(IOPort);
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Configurable IO Pin Demo");
  Serial.println("-----------------------------");

  pinMode(IOPort, OUTPUT);

  SerialCommandHandler.AddCommand(F("SetIOPin"), Cmd_SetIOPin);
}

void loop()
{
  SerialCommandHandler.Process();

  // Toggle the IO Pin
  uint32_t uNow = millis();
  if (uNow - LastToggleTime < OnTime)
    digitalWrite(IOPort, HIGH);
  else
    digitalWrite(IOPort, LOW);

  if (uNow - LastToggleTime > OnTime + OffTime)
    LastToggleTime = uNow;
}
