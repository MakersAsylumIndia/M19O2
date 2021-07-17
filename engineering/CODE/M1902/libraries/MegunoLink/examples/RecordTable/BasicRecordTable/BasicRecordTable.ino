/* ************************************************************************
   Basic Demonstration of MegunoLinks Record Table
   This example demonstrates MegunoLinks Record Table visualiser by outputting
   10 lines of data every 5 seconds. The table is then reset and 10 new records
   are generated.

   Visit:
   * http://www.MegunoLink.com to download MegunoLink.
   * https://www.megunolink.com/documentation/table/record-table/ to learn
     more about record tables. 
   ************************************************************************ */

#include "MegunoLink.h"
#include "ArduinoTimer.h"

RecordTable MyTable;
ArduinoTimer TableRefreshTimer;

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.print("MegunoLink Record Table Basic Example\n\n\n");

  MyTable.SetNumberOfDecimalPlaces(5);
}

void loop()
{

  if (TableRefreshTimer.TimePassed_Milliseconds(5000))
  {
    float AValue = 1e-5;
    MyTable.ClearAllRows();
    for (int i = 0; i < 10; i++)
    {
      bool IsOdd = (i%2) != 0;
      MyTable.AddRow(i, IsOdd, millis(), SpecialParameters::CurrentTime, rand(), "fish", analogRead(0), AValue);
      AValue = AValue * 2;
      delay(100);
    }
  }

}
