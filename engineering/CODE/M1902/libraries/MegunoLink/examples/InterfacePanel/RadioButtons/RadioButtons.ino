/************************************************************************************************
Example Description
In this example we process commands sent from MegunoLink when radio buttons on an Interface Panel
change state.
 
More Information
*  https://www.megunolink.com/documentation/interface-panel/

This Example Requires:
*  The MegunoLink arduino library https://www.megunolink.com/documentation/arduino-integration/

MegunoLink Interface
You can download a pre-made interface from here:
https://github.com/Megunolink/MLP/raw/master/examples/InterfacePanel/RadioButtons/RadioButtons.mlpz
 
You can find out more about MegunoLink and download a free trial from here
https://www.megunolink.com/
https://www.megunolink.com/download/
************************************************************************************************/

#include "MegunoLink.h"
#include "CommandHandler.h"


bool RBLow = true;
bool RBMed = false;
bool RBHigh = false;

CommandHandler<> SerialCommandHandler;


void Cmd_RadioButtonLow(CommandParameter &Parameters)
{
  RBLow = Parameters.NextParameterAsInteger();
}

void Cmd_RadioButtonMedium(CommandParameter &Parameters)
{
  RBMed = Parameters.NextParameterAsInteger();
}

void Cmd_RadioButtonHigh(CommandParameter &Parameters)
{
  RBHigh = Parameters.NextParameterAsInteger();
}

void Cmd_Print(CommandParameter &Parameters)
{
  Serial.println("\n\n");
  Serial.print("RB Low  = "); Serial.println(RBLow);
  Serial.print("RB Med  = "); Serial.println(RBMed);
  Serial.print("RB High = "); Serial.println(RBHigh);
}

void setup()
{
  Serial.begin(9600);
  Serial.println("MegunoLink Pro Radio Button Demo");
  Serial.println("-----------------------------");

  SerialCommandHandler.AddCommand(F("RBLow"), Cmd_RadioButtonLow);
  SerialCommandHandler.AddCommand(F("RBMed"), Cmd_RadioButtonMedium);
  SerialCommandHandler.AddCommand(F("RBHigh"), Cmd_RadioButtonHigh);
  SerialCommandHandler.AddCommand(F("Print"), Cmd_Print);

}

void loop()
{
  SerialCommandHandler.Process();
}