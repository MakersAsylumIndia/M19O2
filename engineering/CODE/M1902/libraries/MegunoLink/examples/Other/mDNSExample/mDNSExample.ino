/* 
  mDNSExample - 
    Based on example Transparent UART to Telnet Server for esp8266

  See: https://www.megunolink.com/documentation/connecting/mdns-browser/
  for more information

  Copyright (c) 2018 Number Eight Innovation Limited.
  Copyright (c) 2015 Hristo Gochkov. All rights reserved.
 
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#if defined(ARDUINO_ARCH_ESP32)
#include "WiFi.h"
#include <ESPmDNS.h>
#elif defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#else
#error Not compatible
#endif

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

// Maximum number of clients that can connect to this device
#define MAX_SRV_CLIENTS 1
WiFiServer server(23);
WiFiClient serverClients[MAX_SRV_CLIENTS];

/* Returns a semi-unique id for the device. The id is based
*  on part of a MAC address or chip ID so it won't be 
*  globally unique. */
uint16_t GetDeviceId()
{
#if defined(ARDUINO_ARCH_ESP32)
  return ESP.getEfuseMac();
#else
  return ESP.getChipId();
#endif
}

/* Append a semi-unique id to the name template */
String MakeMine(const char *NameTemplate)
{
  uint16_t uChipId = GetDeviceId();
  String Result = String(NameTemplate) + String(uChipId, HEX);
  return Result;
}

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

void AdvertiseServices(const char *MyName)
{
  if (MDNS.begin(MyName))
  {
    Serial.println(F("mDNS responder started"));
    Serial.print(F("I am: "));
    Serial.println(MyName);

    // Add service to MDNS-SD
    MDNS.addService("n8i-mlp", "tcp", 23);
  }
  else
  {
    while (1) 
    {
      Serial.println(F("Error setting up MDNS responder"));
      delay(1000);
    }
  }
}

/*  Check if there are any new clients wanting to connect. If there is 
 *  space to keep track of them, do so. Otherwise reject the connection. 
 */
void CheckForNewClients()
{
  uint8_t i;
  if (server.hasClient())
  {
    for(i = 0; i < MAX_SRV_CLIENTS; i++)
    {
      //find free/disconnected spot
      if (!serverClients[i] || !serverClients[i].connected())
      {
        if(serverClients[i]) serverClients[i].stop();
        serverClients[i] = server.available();
        Serial.print("New client: "); Serial.print(i);
        break;
      }
    }
    //no free/disconnected spot so reject
    if ( i == MAX_SRV_CLIENTS) 
    {
       WiFiClient serverClient = server.available();
       serverClient.stop();
       Serial.println("Connection rejected ");
    }
  }
}

/* Process the data received from any connected clients. Here
 * we just send any data received out the local serial port. 
 */
void ProcessConnectedClientData()
{
  // check clients for data. Any data that is received is sent
  // out the local serial port. 
  for(int i = 0; i < MAX_SRV_CLIENTS; i++)
  {
    if (serverClients[i] && serverClients[i].connected())
    {
      if (serverClients[i].available())
      {
        //get data from the telnet client and push it to the UART
        while (serverClients[i].available())
        {
          char Received = serverClients[i].read();
          ProcessClientData(serverClients[i], Received);
        }
      }
    }
  }
}

/* Do something with Data, which we just received from Client. */
void ProcessClientData(WiFiClient &Client, char Data)
{
  Serial.write(Data);
}

void setup()
{
  Serial.begin(9600);
  Serial.println(F("mDNS Test"));
  Serial.println(F("---------"));

  ConnectToWiFi();

  String MyName = MakeMine("WiFiSensor");
  AdvertiseServices(MyName.c_str());

  // Start the TCP server
  server.begin();
  server.setNoDelay(true);
  
  Serial.println("Ready.");
}

void loop() 
{
  MDNS.update();

  CheckForNewClients();

  ProcessConnectedClientData();

  // Check UART for data. Any serial data received is sent to 
  // connected TCP clients.
  if(Serial.available())
  {
    size_t len = Serial.available();
    uint8_t sbuf[len];
    Serial.readBytes(sbuf, len);
    //push UART data to all connected telnet clients
    for(int i = 0; i < MAX_SRV_CLIENTS; i++)
    {
      if (serverClients[i] && serverClients[i].connected()){
        serverClients[i].write(sbuf, len);
        delay(1);
      }
    }
  }
}
