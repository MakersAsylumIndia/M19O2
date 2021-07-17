#include "MegunoLink.h"
#include "CommandHandler.h"

#define LEDPIN 9 //Change this to the pin your LED is on.
boolean LEDState = false;

CommandHandler<> SerialCommandHandler;


void Cmd_ToggleLED(CommandParameter &Parameters)
{
  Serial.println("Got ToggleLED command.");
  LEDState = !LEDState;
  digitalWrite(LEDPIN,LEDState);
}

void setup()
{
  Serial.begin(9600);
  Serial.println("MegunoLink Pro - Turning and LED on and off");
  Serial.println("-----------------------------");
  
  SerialCommandHandler.AddCommand(F("ToggleLED"), Cmd_ToggleLED);

  pinMode(LEDPIN,OUTPUT);
  digitalWrite(LEDPIN,LEDState);
}

void loop()
{
  SerialCommandHandler.Process();
}
