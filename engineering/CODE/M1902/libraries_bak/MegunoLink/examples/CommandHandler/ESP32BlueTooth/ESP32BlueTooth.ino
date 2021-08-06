/************************************************************************************************
Example Description
In this example we demonstrate our command handler for processing commands sent over the ESP32's
Bluetooth connection.
 
More Information
*  https://www.megunolink.com/documentation/getting-started-process-serial-commands/
*  https://www.megunolink.com/documentation/arduino-libraries/serial-command-handler/

This Example Requires:
*  The MegunoLink arduino library https://www.megunolink.com/documentation/arduino-integration/
*  The Pstring Library http://arduiniana.org/libraries/pstring/
*  The EtherCard Library https://github.com/njh/EtherCard. 
 
MegunoLink Interface
You can download a pre-made interface from here:
https://github.com/Megunolink/MLP/raw/master/examples/CommandHandler/ESP32BlueTooth/ESP32BlueTooth.mlpz
 
You can find out more about MegunoLink and download a free trial from here
https://www.megunolink.com/
https://www.megunolink.com/download/
************************************************************************************************/

 /* To connect with the ESP32 in windows, open the Settings application;
    choose Devices and Add Bluetooth or other device. */ 
 
#include "BluetoothSerial.h"
#include "CommandHandler.h"
#include "ArduinoTimer.h"

BluetoothSerial SerialBT;
CommandHandler<> Cmds(SerialBT);

void Cmd_ConnectionColor(CommandParameter &p)
{
  SerialBT.println("Blue");
}

void setup() 
{
  Serial.begin(9600);
  Serial.println("Serial Bluetooth test");
  SerialBT.begin("ESP32");
  Cmds.AddCommand(F("GetConnectionColor"), Cmd_ConnectionColor);
}

void loop() 
{
  Cmds.Process();
}
