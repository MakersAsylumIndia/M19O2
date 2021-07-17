#include "MegunoLink.h"
#include "CommandHandler.h"


int NumberOfTurtles = 0;

CommandHandler<> SerialCommandHandler;

void Cmd_GetTurtleCount(CommandParameter &Parameters)
{
  Parameters.GetSource().print(F("Number of turtles = "));
  Parameters.GetSource().println(NumberOfTurtles);
}

void Cmd_SetTurtleCount(CommandParameter &Parameters)
{
  NumberOfTurtles=Parameters.NextParameterAsInteger();
}

void setup()
{
  Serial.begin(9600);
  Serial.println("MegunoLink Pro Turtle Monitor");
  Serial.println("-----------------------------");
  
  SerialCommandHandler.AddCommand(F("SetTurtleCount"), Cmd_SetTurtleCount);
  SerialCommandHandler.AddCommand(F("GetTurtleCount"), Cmd_GetTurtleCount);
  
}

void loop()
{
  SerialCommandHandler.Process();
}
