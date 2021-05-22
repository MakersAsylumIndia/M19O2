/****************************************************************************************	
**  This is example LINX firmware for use with the chipKIT WiFIRE with the WIFI
**  interface enabled.
**
**  For more information see:           www.labviewmakerhub.com/linx
**  For support visit the forums at:    www.labviewmakerhub.com/forums/linx
**  
**  Written By Sudharsan Sukumar
**
**  BSD2 License.
****************************************************************************************/

//Include All Peripheral Libraries Used By LINX
#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Servo.h>

#include <MRF24G.h>
#include <DEIPcK.h>
#include <DEWFcK.h>

//Include Device Specific Header From Sketch>>Import Library (In This Case LinxChipkitMax32.h)
//Also Include Desired LINX Listener From Sketch>>Import Library 
#include <LinxChipkitWifire.h>
#include <LinxChipkitWifiListener.h>
 
//Create A Pointer To The LINX Device Object We Instantiate In Setup()
LinxChipkitWifire* LinxDevice;

//Initialize LINX Device And Listener
void setup()
{
  //Instantiate The LINX Device
  LinxDevice = new LinxChipkitWifire();
  
  //The LINX Serial Listener Is Included In WIFI Listener And Pre Instantiated.  This Is Necessary For Configuring Wifi Settings.
  LinxSerialConnection.Start(LinxDevice, 0);  
  
  //Start Wifi Listener.  Settings (IP, SSID, etc) Will Be Read From Non Volatile Storage And Can Be Set Using LINX VIs Via USB
  LinxWifiConnection.Start(LinxDevice);  
}

void loop()
{
  //Listen For New Packets From LabVIEW
  LinxWifiConnection.CheckForCommands();
  LinxSerialConnection.CheckForCommands();
  
  //Your Code Here, But It will Slow Down The Connection With LabVIEW
}


