/************************************************************************************************
Example Description
In this example we demonstrate how you can use MegunoLink's Interface Panel and our command handler
library to identify when your device is connected to MegunoLink and when its not. This allows you
to change the behaviour of your device dynamically. For example when its plugged in you might want
to enter a configuration mode and when its unplugged it should run the program as usual.
 
More Information
*  https://www.megunolink.com/documentation/build-arduino-interface/
*  https://www.megunolink.com/documentation/interface-panel/
*  https://www.megunolink.com/documentation/arduino-libraries/serial-command-handler/
 
This Example Requires:
*  The MegunoLink arduino library https://www.megunolink.com/documentation/arduino-integration/
*  The Pstring Library http://arduiniana.org/libraries/pstring/

MegunoLink Interface
You can download a pre-made interface from here:
https://github.com/Megunolink/MLP/raw/master/examples/InterfacePanel/IsMegunoLinkConnected/IsMegunoLinkConnected.mlpz
 
You can find out more about MegunoLink and download a free trial from here
https://www.megunolink.com/
https://www.megunolink.com/download/
************************************************************************************************/

#include "MegunoLink.h"
#include "ArduinoTimer.h"
#include "CommandHandler.h"

InterfacePanel MyPanel;
ArduinoTimer MLPCheckTimer;
ArduinoTimer StatusPrintTimer;
CommandHandler<> MyCommandHanlder;

uint32_t LastAnnounceTime = 0;
uint32_t MLPNotThereTimeout = 10*1000; //mS
bool IsMLPThere = false;

// This function is called whenever MegunoLink successfully responds
void Cmd_MegunoLinkIsHere(CommandParameter &Parameters)
{
  LastAnnounceTime = millis();
}


void setup()
{
  Serial.begin(9600);
  MyCommandHanlder.AddCommand(F("MLIsHere"), Cmd_MegunoLinkIsHere);
}


void loop()
{
  MyCommandHanlder.Process();

  // Sends a message to MegunoLink every 5 seconds to check connection
  if (MLPCheckTimer.TimePassed_Seconds(5))
  {
    MyPanel.CallCommand(F("Announce"));
  }

  // If the arduino has herd from MegunoLink in the last timeout period then it returns true, else false.
  IsMLPThere = ((millis() - LastAnnounceTime) > MLPNotThereTimeout) ? false : true;

  // This prints the result every 1 second to confirm its working
  if (StatusPrintTimer.TimePassed_Milliseconds(1000))
  {
    Serial.print("Is MLP there? ");
    Serial.println(IsMLPThere);
  }
}
