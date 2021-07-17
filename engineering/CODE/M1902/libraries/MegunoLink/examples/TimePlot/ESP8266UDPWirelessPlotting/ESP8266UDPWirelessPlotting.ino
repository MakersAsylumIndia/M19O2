/************************************************************************************************
Example Description
Data is sent from an ESP8266 wifi module over UDP to MegunoLink where it is plotted in real time

More Information
*  http://www.megunolink.com/documentation/plotting/

This Example Requires:
*  The MegunoLink arduino library https://www.megunolink.com/documentation/arduino-integration/
*  The WiFi Manager library https://github.com/tzapu/WiFiManager
*  The ESP8266 arduino library and files https://github.com/esp8266/Arduino


MegunoLink Interface
You can download a pre-made interface from here:
https://github.com/Megunolink/MLP/raw/master/examples/TimePlot/ESP8266UDPWirelessPlotting/ESP8266UDPWirelessPlotting.mlpz

You can find out more about MegunoLink and download a free trial from here
https://www.megunolink.com/
https://www.megunolink.com/download/
************************************************************************************************/

#include "DNSServer.h"
#include "ESP8266WebServer.h"
#include "WiFiManager.h"
#include "ESP8266WiFi.h"
#include "WiFiUdp.h"

#include "MegunoLink.h"

const int SourcePort = 52790;
const int DestinationPort = 52791;
const IPAddress DestinationIp(255, 255, 255, 255);

WiFiManager ConnectionManager;
WiFiUDP UdpConnection;

void setup()
{
  Serial.begin(9600);
  Serial.println("WiFi Test");

  Serial.println("Connecting");
  ConnectionManager.autoConnect();
  Serial.println("Connected");

  UdpConnection.begin(SourcePort);
}

void loop()
{
  // Send a message
  UdpConnection.beginPacket(DestinationIp, DestinationPort);
  UdpConnection.println("A wireless hello");
  UdpConnection.endPacket();

  // Send data for plotting
  UdpConnection.beginPacket(DestinationIp, DestinationPort);
  TimePlot Plot("ADC", UdpConnection);
  Plot.SendData("A0", analogRead(A0));
  UdpConnection.endPacket();

  delay(500); // ms
}