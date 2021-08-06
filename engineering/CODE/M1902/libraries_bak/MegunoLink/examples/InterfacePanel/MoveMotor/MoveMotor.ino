/* ************************************************************************
Move Motor 
This program demonstrates how MegunoLink's Interface Panel and our command
handler Arduino library can be used to control a stepper motor.

The example folder also contains a MegunoLink project, with an Interface
Panel to control the motor.
Visit:
* http://www.MegunoLink.com to download MegunoLink.
************************************************************************ */
#include "CommandHandler.h"

CommandHandler<> SerialCommands;

const int PinA = 7;
const int PinB = 6;

void setup() 
{
  pinMode(PinA, OUTPUT);
  pinMode(PinB, OUTPUT);
  Stop();
  
  Serial.begin(9600);
  Serial.println("Motor Mover");

  SerialCommands.AddCommand(F("Forward"), Cmd_Forward);
  SerialCommands.AddCommand(F("Reverse"), Cmd_Reverse);
  SerialCommands.AddCommand(F("Stop"), Cmd_Stop);
}

void loop() 
{
  SerialCommands.Process();

}

void Stop()
{
  digitalWrite(PinA, LOW);
  digitalWrite(PinB, LOW);
}

void Move(bool Forward)
{
  if (Forward)
  {
    digitalWrite(PinA, LOW);
    digitalWrite(PinB, HIGH);
  }
  else
  {
    digitalWrite(PinA, HIGH);
    digitalWrite(PinB, LOW);
  }
}

void Cmd_Forward(CommandParameter &p)
{
  Serial.println("Forward!");
  Move(true);
}

void Cmd_Reverse(CommandParameter &p)
{
  Serial.println("Reverse!");
  Move(false);
}

void Cmd_Stop(CommandParameter &p)
{
  Serial.println("Whoa!");
  Stop();
}

