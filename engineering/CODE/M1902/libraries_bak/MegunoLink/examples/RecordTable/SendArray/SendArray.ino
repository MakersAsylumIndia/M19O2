/* ************************************************************************
   Send an array of data to a MegunoLink Record Table
   This example demonstrates sending an array of data read from the 
   Arduino analog inputs to a MegunoLink Record Table. The analog 
   measurements will be random unless you connect sensors to the analog 
   inputs. 

   Visit:
   * http://www.MegunoLink.com to download MegunoLink.
   * https://www.megunolink.com/documentation/table/record-table/ to learn
     more about record tables. 
   ************************************************************************ */

#include "MegunoLink.h"
#include "ArduinoTimer.h"

RecordTable MyTable;
ArduinoTimer SendDataTimer;

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.print("MegunoLink Record Table Array Example\n\n\n");
}

void loop()
{
  if (SendDataTimer.TimePassed_Milliseconds(5000))
  {
    const int NumberOfChannels = 4;
    int Measurements[NumberOfChannels];

    // Read the analog inputs. 
    for(int Channel = 0; Channel < NumberOfChannels; ++Channel)
    {
      // Read analog sensor twice with a small delay to allow 
      // the analog input to stabilize after the multiplexer 
      // switches to a new channel. See:
      // https://forum.arduino.cc/index.php?topic=54976.0
      analogRead(Channel);
      delay(10);
      Measurements[Channel] = analogRead(Channel);
    }

    MyTable.AddRowFromArray(NumberOfChannels, Measurements);
  }
}
