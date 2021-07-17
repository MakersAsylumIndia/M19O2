/* **********************************************************************************************
*  Example program to send CSV data through Megunolinks message system and log data to a text file
*  based on a file name supplied from the Arduino. 
*  ********************************************************************************************** */

#include "MegunoLink.h"
#include "CommandHandler.h"
#include "ArduinoTimer.h"

Message Msg;
CommandHandler<> Cmds;
ArduinoTimer ExperimentTimer;


int SamplingPeriod = 10;
uint32_t ExperimentStartTime = -SamplingPeriod * 1000;


void Cmd_StartExperiment(CommandParameter &p)
{
  SamplingPeriod = p.NextParameterAsInteger();
  Msg.LogTo(p.RemainingParameters());
  ExperimentStartTime = millis();
}

void setup()
{
  Serial.begin(9600);
  Cmds.AddCommand(F("Start"), Cmd_StartExperiment);

}

void loop()
{
  Cmds.Process();
  if ((millis() - ExperimentStartTime) < (SamplingPeriod * 1000))
  {
    Msg.Begin();
    Serial.print(millis() - ExperimentStartTime); //Milliseconds since run started
    Serial.print(",");
    Serial.print(analogRead(A0));
    Serial.print(",");
    Serial.print(analogRead(A1));
    Msg.End();

    delay(100);
  }

}