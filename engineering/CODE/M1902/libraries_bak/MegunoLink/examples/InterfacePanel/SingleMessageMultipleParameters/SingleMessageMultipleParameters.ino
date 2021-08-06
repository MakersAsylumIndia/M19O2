/* ************************************************************************
Single Message Multiple Parameters
This program demonstrates how MegunoLink's Interface Panel and our command
handler Arduino library can be used to update multiple variables using a
single message.

The example folder also contains a MegunoLink project, with an Interface
Panel to update the variables.
Visit:
* http://www.MegunoLink.com to download MegunoLink.
************************************************************************ */

#include "MegunoLink.h"
#include "CommandHandler.h"


float a = 1;
float b = 2;
float c = 3;

CommandHandler<> SerialCommandHandler;
InterfacePanel MyInterface;

void Cmd_GetValues(CommandParameter &Parameters)
{
  Serial.println("\nSending values to interface.");
  MyInterface.SetNumber(F("a"), a);
  MyInterface.SetNumber(F("b"), b);
  MyInterface.SetNumber(F("c"), c);
}

void Cmd_SetValues(CommandParameter &Parameters)
{
  Serial.print("\nUpdating values..");
  a = Parameters.NextParameterAsDouble();
  b = Parameters.NextParameterAsDouble();
  c = Parameters.NextParameterAsDouble();

  Serial.print(" a = "); Serial.print(a);
  Serial.print(" b = "); Serial.print(b);
  Serial.print(" c = "); Serial.println(c);
}

void setup()
{
  Serial.begin(9600);
  delay(1000);
  Serial.println("Single Message Multiple Parameter Example");
  Serial.println("------------------------------------------");

  SerialCommandHandler.AddCommand(F("SetValues"), Cmd_SetValues);
  SerialCommandHandler.AddCommand(F("GetValues"), Cmd_GetValues);

}

void loop()
{
  SerialCommandHandler.Process();
}