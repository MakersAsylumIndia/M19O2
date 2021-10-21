/************************************************************************************************
Example Description
This example is a demonstration of MegunoLink's Interface Panel and Command Handler. It allows us
to adjust the values of variables inside our program and turn on and off and LED. See below for 
the available commands.
 
More Information
*  https://www.megunolink.com/documentation/build-arduino-interface/
*  https://www.megunolink.com/documentation/interface-panel/
*  https://www.megunolink.com/documentation/arduino-libraries/serial-command-handler/
 
This Example Requires:
*  The MegunoLink arduino library https://www.megunolink.com/documentation/arduino-integration/
 
MegunoLink Interface
You can download a pre-made interface from here:
https://github.com/Megunolink/MLP/raw/master/examples/CommandHandler/HandleSerialCommands/HandleSerialCommands.mlpz
 
You can find out more about MegunoLink and download a free trial from here
https://www.megunolink.com/
https://www.megunolink.com/download/
************************************************************************************************/

/*
	All commands start with a ! character and end
    with \r (carrige return). It responds to the following serial commands:
      !SetTurtles n\r\n
        Sets the current turtle count to n (an integer)

      !GetTurtles\r\n
        Prints the current turtle count

      !AddTurtle\r\n
        Adds one to the current turtle count

      !LED on\r\n
        Turns an led on

      !LED off\r\n
        Turns an led off.

*/

#include "CommandHandler.h" // The serial command handler is defined in here. 

// Most Arduino boards have an LED connected to one of the digital output
// pins. For the Uno, this is pin 13. 
const int LEDPin = 13;

// The number of turtles counted to date. 
int NumberOfTurtles = 0;

// The serial command handler. Receives serial data and dispatches 
// recognised commands to functions registered during setup. 
CommandHandler<> SerialCommandHandler;

void PrintTurtleCount()
{
  Serial.print(F("Number of turtles = "));
  Serial.println(NumberOfTurtles);
}

// -----------------------------------------------------------------------
// Command handlers. 
// These functions are called when a serial command is received. 
void Cmd_GetTurtleCount(CommandParameter &Parameters)
{
  PrintTurtleCount();
}

void Cmd_SetTurtleCount(CommandParameter &Parameters)
{
  NumberOfTurtles = Parameters.NextParameterAsInteger();
}

void Cmd_AddTurtle(CommandParameter &Parameters)
{
  NumberOfTurtles = NumberOfTurtles + 1;
}

void Cmd_LED(CommandParameter &Parameters)
{
  const char *State = Parameters.NextParameter();

  if (strcmp(State, "on") == 0)
  {
    digitalWrite(LEDPin, HIGH);
  }
  else
  {
    digitalWrite(LEDPin, LOW);
  }
}

void Cmd_Unknown()
{
  Serial.println(F("I don't know that command. Try another. "));
}

// -----------------------------------------------------------------------
void setup()
{
  Serial.begin(9600);
  Serial.println(F("MegunoLink Pro Turtle Monitor"));
  Serial.println(F("-----------------------------"));
  PrintTurtleCount();

  Serial.println(F("Supported commands: "));
  Serial.println(F("!SetTurtles n\r\n"));
  Serial.println(F("  Sets the current turtle count to n(an integer)"));
  Serial.println();
  Serial.println(F("!GetTurtles\r\n"));
  Serial.println(F("  Prints the current turtle count"));
  Serial.println();
  Serial.println(F("!AddTurtle\r\n"));
  Serial.println(F("  Adds one to the current turtle count"));
  Serial.println();
  Serial.println(F("!LED on\r\n"));
  Serial.println(F("  Turns an led on"));
  Serial.println();
  Serial.println(F("!LED off\r\n"));
  Serial.println(F("  Turns an led off."));

  // Setup the commands the handler will respond to. The first parameter
  // is the command name, the second is the function that will be called
  // when the command is received. Note that each command name is 
  // inside F(""). This places the command text in program memory to
  // save RAM. 
  SerialCommandHandler.AddCommand(F("SetTurtles"), Cmd_SetTurtleCount);
  SerialCommandHandler.AddCommand(F("GetTurtles"), Cmd_GetTurtleCount);
  SerialCommandHandler.AddCommand(F("AddTurtle"), Cmd_AddTurtle);
  SerialCommandHandler.AddCommand(F("LED"), Cmd_LED);

  SerialCommandHandler.SetDefaultHandler(Cmd_Unknown);

  pinMode(LEDPin, OUTPUT);
}

void loop()
{
  // Call the serial command handler's process function. It will receive
  // the serial data and call the registered function when a 
  // recognized command is received. 
  SerialCommandHandler.Process();
}
