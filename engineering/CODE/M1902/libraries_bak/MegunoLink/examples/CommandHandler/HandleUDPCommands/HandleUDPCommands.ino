/* ******************************************************************************
 *  Demonstrate processing commands sent over a UDP connection
 *  See "Handle UDP Commands.mlx" file in example's folder for a MegunoLink 
 *  project to send commands and show the results. 
 *  Download a free trial of MegunoLink from www.MegunoLink.com
 *  Check out: https://www.megunolink.com/documentation/connecting/udp-connections/
 *  for more information on UDP connections. 
 *  
 *  Check out this video for a demo of the example in action  https://youtu.be/nvmuvDnGFjU
 */

#include "DNSServer.h"
#include "ESP8266WebServer.h"
#include "ESP8266WiFi.h"
#include "WiFiUdp.h"

#include "MegunoLink.h"
#include "CommandDispatcher.h"

const int SourcePort = 52790;
const int DestinationPort = 52791;
const IPAddress DestinationIp(255, 255, 255, 255);

WiFiUDP UdpConnection;

#define USEWIFICONFIGFILE
#ifdef USEWIFICONFIGFILE
// Include SSID and password from a library file. For more information see:
// https://www.megunolink.com/articles/wireless/how-do-i-connect-to-a-wireless-network-with-the-esp32/
#include "WiFiConfig.h"
#else
// Option 2
const char *SSID = "Your SSID";
const char *WiFiPassword = "Your Password";
#endif

// -------------------------------------------------------------------------------
// Handling commands
const int MaxCommands = 10; 
CommandDispatcher<MaxCommands> Commands;

void Cmd_GetADCValue(CommandParameter &p)
{
  uint16_t ADCValue = analogRead(0);

  UdpConnection.beginPacket(DestinationIp, DestinationPort);
  Table MLPTable("", UdpConnection); //"" is an empty channel. Needed when specifying a port other than standard serial.
  MLPTable.SendData("A0", ADCValue);  
  UdpConnection.endPacket();
}

void Cmd_Hello(CommandParameter &p)
{
  UdpConnection.beginPacket(DestinationIp, DestinationPort);
  UdpConnection.println(F("Hello World!"));
  UdpConnection.endPacket();
}

// Replaces the first '\r' or '\n' character with a null terminating the string. 
void TruncateAtNewLine(char *Buffer)
{
  for(char *Current = Buffer; *Current; ++Current)
  {
    if (*Current == '\r' || *Current == '\n')
    {
      *Current = '\0';
      break;
    }
  }
}
// -------------------------------------------------------------------------------
// Arduino entry points
void setup() 
{
  delay(2000);
  Serial.begin(9600);
  Serial.println("WiFi Receive Test");

  Serial.println("Connecting");
  WiFi.begin(SSID, WiFiPassword);
  Serial.println("Connected");
  
  UdpConnection.begin(SourcePort);

  Commands.AddCommand(F("Hello"), Cmd_Hello);
  Commands.AddCommand(F("GetADCValue"), Cmd_GetADCValue);
}

void loop() 
{
  int PacketSize = UdpConnection.parsePacket();
  if (PacketSize != 0)
  {
    char ReceiveBuffer[100];
    int Length = UdpConnection.read(ReceiveBuffer, sizeof(ReceiveBuffer));
    if (Length >= 0 && Length < sizeof(ReceiveBuffer))
    {
      ReceiveBuffer[Length] = 0; 
    }
    else
    {
      Serial.println(F("Message too big!"));
      Length = 0; 
    }

    Serial.print(F("Received packet of "));
    Serial.print(PacketSize);
    Serial.print(F(" bytes, from "));
    Serial.print(UdpConnection.remoteIP());
    if (Length > 0)
    {
      Serial.print(F(" content: "));
      Serial.println(ReceiveBuffer);
    }

    if (Length >0 && ReceiveBuffer[0] == '!')
    { 
      TruncateAtNewLine(ReceiveBuffer);
      Commands.DispatchCommand(ReceiveBuffer + 1, UdpConnection);
    }
  }
}
