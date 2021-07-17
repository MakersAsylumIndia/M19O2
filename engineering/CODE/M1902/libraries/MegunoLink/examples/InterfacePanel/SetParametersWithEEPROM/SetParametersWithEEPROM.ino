#include "MegunoLink.h"
#include "CommandHandler.h"
#include "EEPROMStore.h" 


//EEPROM Storage
struct TurtleSettings
{
  int NumberOfTurtles;

  void Reset()
  {
    NumberOfTurtles = 0; // Amount of time the LED remains on [milliseconds]
  }
};
EEPROMStore<TurtleSettings> TurtleConfig;



//Command Handler
CommandHandler<> SerialCommandHandler;
void Cmd_GetTurtleCount(CommandParameter &Parameters)
{
  Parameters.GetSource().print(F("Number of turtles = "));
  Parameters.GetSource().println(TurtleConfig.Data.NumberOfTurtles);
}

void Cmd_SetTurtleCount(CommandParameter &Parameters)
{
  TurtleConfig.Data.NumberOfTurtles=Parameters.NextParameterAsInteger();
  TurtleConfig.Save(); //Save to EEPROM
}

void setup()
{
  Serial.begin(9600);
  Serial.println("MegunoLink Pro Turtle Monitor");
  Serial.println("-----------------------------");
  
  TurtleConfig.Load(); //Restore from EEPROM
  
  SerialCommandHandler.AddCommand(F("SetTurtleCount"), Cmd_SetTurtleCount);
  SerialCommandHandler.AddCommand(F("GetTurtleCount"), Cmd_GetTurtleCount);
  
}

void loop()
{
  SerialCommandHandler.Process();
}
