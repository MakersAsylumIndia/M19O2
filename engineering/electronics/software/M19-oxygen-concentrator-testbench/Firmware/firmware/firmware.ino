/****************************************************************************************	
**  This is example LINX firmware for use with the Arduino Uno with the serial 
**  interface enabled.
**
**  For more information see:           www.labviewmakerhub.com/linx
**  For support visit the forums at:    www.labviewmakerhub.com/forums/linx
**  
* 
**  Written By Sam Kristoff
**
**  BSD2 License.
****************************************************************************************/

//Include All Peripheral Libraries Used By LINX
#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Servo.h>

//Include Device Specific Header From Sketch>>Import Library (In This Case LinxChipkitMax32.h)
//Also Include Desired LINX Listener From Sketch>>Import Library (In This Case LinxSerialListener.h)
#include <LinxArduinoUno.h>
#include <LinxSerialListener.h>
 
//Create A Pointer To The LINX Device Object We Instantiate In Setup()
LinxArduinoUno* LinxDevice;


//Custom libraries & declarations
#include "DHT.h"
#include <Adafruit_ADS1X15.h>



#define DHTPIN 2
#define DHTTYPE DHT22

Adafruit_ADS1X15 ads;
DHT dht(DHTPIN, DHTTYPE);

int readOxygenValue();
int readDHT22();


//Initialize LINX Device And Listener
void setup()
{
  //Instantiate The LINX Device
  LinxDevice = new LinxArduinoUno();
  
  //The LINXT Listener Is Pre Instantiated, Call Start And Pass A Pointer To The LINX Device And The UART Channel To Listen On
  LinxSerialConnection.Start(LinxDevice, 0);  
  LinxSerialConnection.AttachCustomCommand(0, readOxygenValue);
  LinxSerialConnection.AttachCustomCommand(1, readDHT22);
  ads.setGain(GAIN_SIXTEEN);
  ads.begin(); 
  dht.begin();
}

void loop()
{
  //Listen For New Packets From LabVIEW
  LinxSerialConnection.CheckForCommands();


}


int readOxygenValue(unsigned char numInputBytes, unsigned char* input, unsigned char* numResponseBytes, unsigned char* response)
{
   
  unsigned int analogValue = 0;
  
	response[0] = 0x00;
	response[1] = 0x00;


	analogValue = ads.readADC_SingleEnded(0);	

	response[0] |= analogValue>>8;  //MSB
	response[1] |= analogValue & 0000000011111111; //LSB
	

	*numResponseBytes = 2;

  return 0;

}

int readDHT22(unsigned char numInputBytes, unsigned char* input, unsigned char* numResponseBytes, unsigned char* response)
{
   
  response[0] = 0x00;
  response[1] = 0x00;

  unsigned int h = dht.readHumidity();
  unsigned int t = dht.readTemperature();
    if (isnan(h) || isnan(t)) {
    return 0;
  }

  response[0] = h;
  response[1] = t;
  // response[0] |= h>>8;  //MSB
  // response[1] |= h & 0000000011111111; //LSB
  

  *numResponseBytes = 2;

  return 0;

}