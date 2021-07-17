#include "MegunoLink.h"
#include "CommandHandler.h" 
#include "ArduinoTimer.h"

InterfacePanel ParametersPanel;
CommandHandler<> SerialCommandHandler;

long LastBlink = 0; // Time we last blinked the LED
int OnTime = 10; // Amount of time the LED remains on [milliseconds]
int OffTime = 100; // Amount of time the LED remains off [milliseconds]

bool GotOnTime = false; // True when MegunoLink has supplied the on time value.
bool GotOffTime = false; // True when MegunoLink has supplied the off time value. 
ArduinoTimer RequestTimer;  // Timer to periodically request parameters

void setup() 
{
  Serial.begin(9600);
  Serial.println(F("Blink Parameter Request Individual Parameters"));
  Serial.println(F("============================================="));

  SerialCommandHandler.AddCommand(F("numOnTime.Value"), Cmd_SetOnTime);
  SerialCommandHandler.AddCommand(F("numOffTime.Value"), Cmd_SetOffTime);
  SerialCommandHandler.AddCommand(F("ListAll"), Cmd_ListParameters);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() 
{
  // Check for serial commands and dispatch them.
  SerialCommandHandler.Process();

  // Update the LED
  if (GotOnTime && GotOffTime)
  {
    uint32_t uNow = millis();

    digitalWrite(LED_BUILTIN, (uNow - LastBlink < OnTime) ? HIGH : LOW); 
    if ((uNow - LastBlink) > (OnTime + OffTime))
    {
      LastBlink = uNow;
    }
  }

  // Request parameters from MegunoLink. 
  if (RequestTimer.TimePassed_Seconds(10))
  {
    RequestParameters();
  }
}

void RequestParameters()
{
  Serial.println(F("Requesting parameters"));
  ParametersPanel.GetValue(F("numOnTime"), F("Value"));
  ParametersPanel.GetValue(F("numOffTime"), F("Value"));
}

void Cmd_SetOnTime(CommandParameter& Parameters)
{
  OnTime = Parameters.NextParameterAsInteger(OnTime);
  GotOnTime = true;
}

void Cmd_SetOffTime(CommandParameter& Parameters)
{
  OffTime = Parameters.NextParameterAsInteger(OffTime);
  GotOffTime = true;
}

void Cmd_ListParameters(CommandParameter& Parameters)
{
  Serial.print(F("OnTime [ms]="));
  Serial.println(OnTime);
  Serial.print(F("OffTime [ms]="));
  Serial.println(OffTime);
}
