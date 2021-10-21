#include "MegunoLink.h"
#include "CommandProcessor.h"


int NumberOfTurtles = 0;

CommandDispatcher<> CommonCommands;
CommandProcessor<> SerialCommandHandler(CommonCommands, Serial);
CommandProcessor<> Serial2CommandHandler(CommonCommands, Serial2);

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
  
  Serial2.begin(4800);
  
  CommonCommands.AddCommand(F("SetTurtleCount"), Cmd_SetTurtleCount);
  CommonCommands.AddCommand(F("GetTurtleCount"), Cmd_GetTurtleCount);
  
}

void loop()
{
  SerialCommandHandler.Process();
  Serial2CommandHandler.Process();
}
