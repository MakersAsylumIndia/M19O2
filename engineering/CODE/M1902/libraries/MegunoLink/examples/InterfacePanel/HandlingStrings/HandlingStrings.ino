/************************************************************************************************
Example Description
In this example we demonstrate how to handle strings with our command handler. This is great if
you want to be able to send your device a password or SSID to establish a network connection or
maybe adjust some text on a display.
 
More Information
*  https://www.megunolink.com/documentation/build-arduino-interface/
*  https://www.megunolink.com/documentation/interface-panel/
*  https://www.megunolink.com/documentation/arduino-libraries/serial-command-handler/
 
This Example Requires:
*  The MegunoLink arduino library https://www.megunolink.com/documentation/arduino-integration/
*  The Pstring Library http://arduiniana.org/libraries/pstring/

MegunoLink Interface
You can download a pre-made interface from here:
https://github.com/Megunolink/MLP/raw/master/examples/InterfacePanel/HandlingStrings/HandlingStrings.mlpz
 
You can find out more about MegunoLink and download a free trial from here
https://www.megunolink.com/
https://www.megunolink.com/download/
************************************************************************************************/

#include "MegunoLink.h"
#include "CommandHandler.h"
#include "PString.h" //This example requires the PString library http://arduiniana.org/libraries/pstring/


CommandHandler<> SerialCommandHandler;

void Cmd_SetSSID(CommandParameter &Parameters)
{
  char buffer[50]; //Create character buffer for PString to use
  PString mystring(buffer, sizeof(buffer)); //Initialise PString object
  mystring.print(Parameters.RemainingParameters()); //Print the characters from MegunoLink into PString buffer
  
  Serial.print("NewSSID:");
  Serial.println(buffer);
}

void setup()
{
  Serial.begin(9600);
  Serial.println("MegunoLink Pro - Handling Strings");
  Serial.println("-----------------------------");

  SerialCommandHandler.AddCommand(F("SetSSID"), Cmd_SetSSID);

}

void loop()
{
  SerialCommandHandler.Process();
}
