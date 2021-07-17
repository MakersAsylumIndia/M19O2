/************************************************************************************************
Example Description
In this example we plot sinewave data on MegunoLink's Time Plot visualiser
 
More Information
*  http://www.megunolink.com/documentation/plotting/
 
This Example Requires:
*  The MegunoLink arduino library https://www.megunolink.com/documentation/arduino-integration/
*  The Pstring Library http://arduiniana.org/libraries/pstring/
*  The EtherCard Library https://github.com/njh/EtherCard. 
 
MegunoLink Interface
You can download a pre-made interface from here:
https://github.com/Megunolink/MLP/raw/master/examples/TimePlot/ENC28J60UDPPlotting/ENC28J60UDPPlotting.mlpz
 
You can find out more about MegunoLink and download a free trial from here
https://www.megunolink.com/
https://www.megunolink.com/download/
************************************************************************************************/


#include "MegunoLink.h"
#include <EtherCard.h>
#include <MegunoLink.h>
#include <ArduinoTimer.h>
#include <PString.h> 

ArduinoTimer PlotTimer;

static byte mymac[] = { 0x1A,0x2B,0x3C,0x4D,0x5E,0x6F };
byte Ethernet::buffer[700];
const int dstPort PROGMEM = 8888; //Match this to the port selected in MegunoLink
const int srcPort PROGMEM = 4321;

const static uint8_t dstIP[] = { 255,255,255,255 }; //Broadcast address. You can replace this with a specific machines IP





void setup()
{
  Serial.begin(9600);
  Serial.println("MegunoLink Example");

  // Change 'SS' to your Slave Select pin, if you arn't using the default pin
  if (ether.begin(sizeof Ethernet::buffer, mymac, SS) == 0)
    Serial.println("Failed to access Ethernet controller");
  if (!ether.dhcpSetup())
    Serial.println("DHCP failed");

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);
  ether.printIp("DNS: ", ether.dnsip);
}


void loop()
{
  // We send udp packets every 200mS. Because it relies on the network interface its hard
  // to have reliable timing.
  if (PlotTimer.TimePassed_Milliseconds(200)) 
  {
    char MessageBuffer[100];
    PString Message(MessageBuffer, sizeof(MessageBuffer));
    TimePlot MyPlot("", Message);

    double dY, dY2;
    float seconds;
    float frequency = 0.1; //Hz
    float phase = 3.141 / 2;

    seconds = (float)millis() / 1000;

    dY = sin(2 * 3.141 * frequency * seconds);
    dY2 = cos(2 * 3.141 * frequency * seconds + phase);

    //Send Data To MegunoLink Pro
    MyPlot.SendData(F("Sinewave"), dY); // Sinewave = series name, dY = data to plot
    MyPlot.SendData(F("Cosinewave"), dY2); // By wrapping strings in F("") we can save ram by storing strings in program memory
    ether.sendUdp(MessageBuffer, Message.length(), srcPort, dstIP, dstPort);
  }
}