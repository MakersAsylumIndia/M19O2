/* **********************************************************************************************
*  Example program to send CSV data through Megunolinks message system. These are separated from the
*  main serial stream and can either be displayed (Message Monitor) or logged (Message Logger).
*  Visit http://www.megunolink.com/documentation/monitoring-data/ to see how to use our message monitor and
*  http://www.megunolink.com/documentation/logging-data/ to use our message logger.
*  ********************************************************************************************** */
#include "MegunoLink.h"

Message MyCSVMessage("Data"); //"Data" = the taget message channel (remember to select this in megunolink)

// When logging make sure you specifiy a path for the logfile and enable logging.

void setup()
{
  Serial.begin(9600);
}



void loop()
{

  // Sends the microcontroller time, and the analog value using a comma separated format 
  // perfect for use with tools like excel and matlab
  MyCSVMessage.Begin();
  Serial.print(millis());
  Serial.print(",");
  Serial.print(analogRead(A0));
  Serial.print(",");
  MyCSVMessage.End();

  delay(100);
}

