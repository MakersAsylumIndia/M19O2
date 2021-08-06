/* ******************************************************************
 *  This sketch sends analog measurements to a MegunoLink TCP Server
 *  Set the Host to the IP address of your computer and Port to the 
 *  port you setup on MegunoLink's TCP Server configuration panel. 
 *  See https://www.megunolink.com/connecting/tcp-server for more information.
 */

#include <ESP8266WiFi.h>
#include "MegunoLink.h"

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

const char *Host = "192.168.15.117";
const unsigned Port = 11000;

// We'll use a single client and remain connected. The Arduino doesn't
// inform the server that the client has disconnected which leaves a 
// lot of dangling connections to manage if we use a new client
// for each message. 
WiFiClient Client;

void ConnectToWiFi()
{
 
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, WiFiPassword);
  Serial.print("Connecting to "); Serial.println(SSID);
 
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(500);
 
    if ((++i % 16) == 0)
    {
      Serial.println(F(" still trying to connect"));
    }
  }
 
  Serial.print(F("Connected. My IP address is: "));
  Serial.println(WiFi.localIP());
}

WiFiClient *GetClient()
{
  if (!Client.connected() && Client.connect(Host, Port))
  {
    return &Client;
  }

  return Client.connected() ? &Client : NULL;
}

void SendMeasurement(unsigned Value)
{
  WiFiClient *pClient = GetClient();

  if (pClient != NULL)
  {
    TimePlot Plot("", *pClient);
    Plot.SendData("ADC Value", Value);
  }
  else
  {
    Serial.println("TCP Connection failed");
  }
}

void setup() 
{
  Serial.begin(9600);

  ConnectToWiFi();
}

void loop() 
{
  unsigned Measurement = analogRead(0);
  SendMeasurement(Measurement);

  delay(1000);
}
