/* ************************************************************************
Servo Control
This program demonstrates how MegunoLink's Interface Panel and our command
handler Arduino library can be used to control a servo.

The example folder also contains a MegunoLink project, with an Interface
Panel to control the parameters.
Visit:
* http://www.MegunoLink.com to download MegunoLink.
************************************************************************ */

#include <Servo.h>
#include "CommandHandler.h"

#define ServoPin 9

Servo MyServo;  // create servo object to control a servo
CommandHandler<> SerialCommandHandler; // create command handler object

void Cmd_SetServoPosition(CommandParameter &Parameters)
{
  int ReceivedPosition = Parameters.NextParameterAsInteger(); // grab sent number as integer

  Serial.print("Position Received: ");
  Serial.println(ReceivedPosition);

  MyServo.write(ReceivedPosition); // sets the servo position according to received value
}

void setup()
{
  Serial.begin(9600);
  delay(1000);
  Serial.print("Setup started..");

  MyServo.attach(ServoPin);  // attaches the servo on pin 9 to the servo object
  SerialCommandHandler.AddCommand(F("SetServoPos"), Cmd_SetServoPosition); // Attach command

  Serial.println("done.");
}

void loop()
{
  SerialCommandHandler.Process(); // Process serial commands
}
