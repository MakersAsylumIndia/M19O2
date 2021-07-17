#include "MegunoLink.h"
#include "CommandHandler.h"
#include "ArduinoTimer.h"

InterfacePanel Panel;
CommandHandler<> SerialCmds;

bool WaitingForPromptResponse = false; 
int CurrentPromptId = 1;
ArduinoTimer PromptTimeout;

void Cmd_BeginMissileLaunch(CommandParameter& p)
{
  if (WaitingForPromptResponse)
  {
    if (PromptTimeout.TimePassed_Seconds(5))
    {
      WaitingForPromptResponse = false;
    }
    else
    {
      Serial.println(F("Missile launch underway. Waiting for confirmation"));
    }
  }

  if (!WaitingForPromptResponse)
  {
    WaitingForPromptResponse = true; 
    Panel.ShowPrompt("MissilePrompt", ++CurrentPromptId, "Do you really want to launch the missile now?");
    PromptTimeout.Reset();
  }
}

void Cmd_HandlePromptResponse(CommandParameter& p)
{
  int PromptId = p.NextParameterAsInteger();
  const char* Response = p.NextParameter();
  if (PromptId == CurrentPromptId)
  {
    WaitingForPromptResponse = false; 
    bool Launch = *Response != '\0' && *Response == 'Y';
    if (Launch)
    {
      Serial.println(F("Fire missiles!!"));
    }
    else
    {
      Serial.println(F("Launch aborted."));
    }
  }
}

void setup() 
{
  Serial.begin(9600);
  Serial.println(F("Missile launch program"));

  SerialCmds.AddCommand(F("BeginLaunch"), Cmd_BeginMissileLaunch);
  SerialCmds.AddCommand(F("PromptResponse"), Cmd_HandlePromptResponse);
}

void loop() 
{
  SerialCmds.Process();
}
