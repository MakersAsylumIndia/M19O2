#include "MegunoLink.h"
#include "CommandHandler.h"
#include "ArduinoTimer.h"

Message Msg;
CommandHandler<> Cmds;
ArduinoTimer ExperimentTimer;


int FileNumber = 1;
int MeasurementsSaved = 1;
const int MeasurementsPerFile = 10;
const int NumberOfRuns = 5;

bool ExperimentRunning;

void StartNewLogFile(int LogFileNumber)
{
  String FileName("Log");
  FileName += LogFileNumber;
  FileName += ".txt";
  Msg.LogTo(FileName);
  MeasurementsSaved = 0;
}

void StoreMeasurement()
{
  auto Value = analogRead(0);
  Msg.Send(millis(), Value);
  ++MeasurementsSaved;
}

void Cmd_StartExperiment(CommandParameter &p)
{
  ExperimentRunning = true;
  FileNumber = 0;
  StartNewLogFile(++FileNumber);
}

void Cmd_StopExperiment(CommandParameter &p)
{
  Msg.StopLogging();
  ExperimentRunning = false; 
}

void setup() 
{
  Serial.begin(9600);
  Cmds.AddCommand(F("Start"), Cmd_StartExperiment);
  Cmds.AddCommand(F("Stop"), Cmd_StopExperiment);
}

void loop()
{
  Cmds.Process();

  if (ExperimentRunning)
  {
    if (ExperimentTimer.TimePassed_Milliseconds(250))
    { 
      StoreMeasurement();
    }

    if (MeasurementsSaved == MeasurementsPerFile)
    {
      ++FileNumber;
      if (FileNumber <= NumberOfRuns)
      { 
        // Start a new experiment run.
        StartNewLogFile(FileNumber);
      }
      else
      {
        // Stop the experiment when all runs have been completed
        ExperimentRunning = false; 
        Msg.StopLogging();
      }
    }
  }
}
