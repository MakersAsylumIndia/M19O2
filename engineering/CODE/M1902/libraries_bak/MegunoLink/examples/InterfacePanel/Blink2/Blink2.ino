/************************************************************************************************
Example Description
This example is a demonstration of MegunoLink's Interface Panel and Command Handler. It builds on
the classic Blink example where the Arduino flashes and LED but adds the ability to control the
on and off time of the flashing.
 
More Information
*  https://www.megunolink.com/documentation/interface-panel/example-blink2-interface-panel/
*  https://www.megunolink.com/documentation/build-arduino-interface/
*  https://www.megunolink.com/documentation/interface-panel/
*  https://www.megunolink.com/documentation/arduino-libraries/serial-command-handler/
 
This Example Requires:
*  The MegunoLink arduino library https://www.megunolink.com/documentation/arduino-integration/
 
MegunoLink Interface
You can download a pre-made interface from here:
https://github.com/Megunolink/MLP/raw/master/examples/InterfacePanel/Blink2/Blink2.mlpz
 
You can find out more about MegunoLink and download a free trial from here
https://www.megunolink.com/
https://www.megunolink.com/download/
************************************************************************************************/

/*
 The following serial commands are supported:
      !OnTime n\r\n
        Sets the amount of time the LED remains on to n [milliseconds]

      !OnTime ?\r\n
        Returns the amount of time the LED remains on [milliseconds]

      !OffTime n\r\n
        Sets the amount of time the LED remains off to n [milliseconds]

      !OffTime ?\r\n
        Returns the amount of time the LED remains off [milliseconds]
*/

#include "CommandHandler.h" 

CommandHandler<> SerialCommandHandler;

long LastBlink = 0; // Time we last blinked the LED
int OnTime = 10; // Amount of time the LED remains on [milliseconds]
int OffTime = 100; // Amount of time the LED remains off [milliseconds]

const int LEDPin = 13; // Pin the LED is attached to 

void setup() 
{
  Serial.begin(9600);
  Serial.println(F("Blink 2.0"));
  Serial.println(F("=========="));

  // Setup the serial commands we can repond to
  SerialCommandHandler.AddCommand(F("OnTime"), Cmd_SetOnTime);
  SerialCommandHandler.AddCommand(F("OffTime"), Cmd_SetOffTime);
  SerialCommandHandler.AddCommand(F("ListAll"), Cmd_ListAll);
  SerialCommandHandler.SetDefaultHandler(Cmd_Unknown);

  pinMode(LEDPin, OUTPUT);
}

void loop() 
{
  // Check for serial commands and dispatch them.
  SerialCommandHandler.Process();

  // Update the LED
  uint32_t uNow = millis();
  if (uNow - LastBlink < OnTime)
    digitalWrite(LEDPin, HIGH);
  else
    digitalWrite(LEDPin, LOW);

  if (uNow - LastBlink > OnTime + OffTime)
    LastBlink = uNow;
}

void Cmd_ListAll(CommandParameter &Parameters)
{
  Serial.print(F("OnTime [ms]="));
  Serial.println(OnTime);
  Serial.print(F("OffTime [ms]="));
  Serial.println(OffTime);
}

void Cmd_SetOnTime(CommandParameter &Parameters)
{
	OnTime = Parameters.NextParameterAsInteger(OnTime);
}

void Cmd_SetOffTime(CommandParameter &Parameters)
{
	OffTime = Parameters.NextParameterAsInteger(OffTime);
}

void Cmd_Unknown()
{
  Serial.println(F("I don't understand"));
}
