/* ************************************************************************
RequestAMatrix

This program demonstrates how MegunoLink's Interface Panel and our command
handler Arduino library can be used to query a matrix of text boxes to retrieve
a configuration that can be used by the program to execute instructions.

The example folder also contains a MegunoLink project, with an Interface
Panel to update the variables.

Visit:
* http://www.MegunoLink.com to download MegunoLink.

This example requires:
* The PString library http://arduiniana.org/libraries/pstring/
* Our MegunoLink Arduino library https://www.megunolink.com/documentation/arduino-library/
************************************************************************ */

#include <MegunoLink.h>
#include <CommandHandler.h>
#include <ArduinoTimer.h>
#include <PString.h>

#define Rows 4

InterfacePanel MyPanel;
CommandHandler<> SerialCommandHandler;
ArduinoTimer RequestRowTimer;

// Structure to store the values sent from MegunoLink
struct Config {
  int p1;
  int p2;
  int p3;
  int p4;
  int delay;
};

int CurrentRow = Rows + 1; //Start at 5 so it doesn't request anything at startup
Config CurConfig[Rows]; //Create array to store each config row


/*----------------Command Hanlder Functions--------------------------*/
//This function processes the row data sent from MegunoLink.
void Cmd_GetProcessTableRow(CommandParameter &Parameters)
{
  int CurRow = Parameters.NextParameterAsInteger();
  if (CurRow != -1 && CurRow <= Rows)
  {
    CurConfig[CurRow - 1].p1 = Parameters.NextParameterAsInteger(CurConfig[CurRow - 1].p1);
    CurConfig[CurRow - 1].p2 = Parameters.NextParameterAsInteger(CurConfig[CurRow - 1].p2);
    CurConfig[CurRow - 1].p3 = Parameters.NextParameterAsInteger(CurConfig[CurRow - 1].p3);
    CurConfig[CurRow - 1].p4 = Parameters.NextParameterAsInteger(CurConfig[CurRow - 1].p4);
    CurConfig[CurRow - 1].delay = Parameters.NextParameterAsInteger(CurConfig[CurRow - 1].delay);
  }
}

void Cmd_StartConfigTransfer(CommandParameter &Parameters)
{
  CurrentRow = 1; //By setting current row to 1 it will begin requesting row data from MegunoLink
}

void Cmd_DumpConfig(CommandParameter &Parameters)
{
  DumpCurConfig();
}


/*----------------------Main Program---------------------------*/
void setup()
{
  Serial.begin(9600);

  SerialCommandHandler.AddCommand(F("Table"), Cmd_GetProcessTableRow);
  SerialCommandHandler.AddCommand(F("StartCfgXfer"), Cmd_StartConfigTransfer);
  SerialCommandHandler.AddCommand(F("DumpConfig"), Cmd_DumpConfig);
}

void loop()
{
  SerialCommandHandler.Process();

  // This rate limits the row requests to MegunoLink. One every 100mS. 
  // This prevents the microcontroller from being overwhelmed.
  if (RequestRowTimer.TimePassed_Milliseconds(100))
  {
    if (CurrentRow <= Rows)
    {
      char buffer[4];
      PString cmd(buffer, sizeof(buffer));
      cmd.print("G");
      cmd.print(CurrentRow);
      MyPanel.CallCommand(cmd); //This sends the request message
      CurrentRow++;
    }
  }
}

// Print out all of the rows in the current config
void DumpCurConfig()
{
  Serial.println("Index P1 P2 P3 P4 Delay");
  for (int i = 0; i < Rows; i++)
  {
    Serial.print(i + 1); Serial.print(" ");
    Serial.print(CurConfig[i].p1);  Serial.print(" ");
    Serial.print(CurConfig[i].p2); Serial.print(" ");
    Serial.print(CurConfig[i].p3); Serial.print(" ");
    Serial.print(CurConfig[i].p4); Serial.print(" ");
    Serial.println(CurConfig[i].delay);
  }
}