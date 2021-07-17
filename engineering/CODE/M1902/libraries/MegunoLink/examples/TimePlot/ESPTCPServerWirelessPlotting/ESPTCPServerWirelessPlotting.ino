/************************************************************************************************
Example Description
Data is sent from an ESP32 or ESP8266 server using a TCP connection to MegunoLink where it is
plotted in real time.

More Information
*  http://www.megunolink.com/documentation/plotting/
*  https://www.megunolink.com/articles/wireless/how-do-i-connect-to-a-wireless-network-with-the-esp32/
*  https://www.megunolink.com/articles/wireless/find-esp32-esp8266-ip-address-with-mdns/

This Example Requires:
*  The MegunoLink arduino library https://www.megunolink.com/documentation/arduino-integration/

MegunoLink Interface
You can download a pre-made interface from here:
https://github.com/Megunolink/MLP/raw/master/examples/TimePlot/ESPTCPServerWirelessPlotting/ESPTCPServerWirelessPlotting.mlpz

You can find out more about MegunoLink and download a free trial from here
https://www.megunolink.com/
https://www.megunolink.com/download/
************************************************************************************************/

#if defined(ARDUINO_ARCH_ESP32)
#include "WiFi.h"
#include <ESPmDNS.h>
#elif defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#else
#error Not compatible
#endif


#include "MegunoLink.h"
#include "ArduinoTimer.h"


#define USEWIFICONFIGFILE
#ifdef USEWIFICONFIGFILE
// Option 1
// Include SSID and password. For more information see:
// https://www.megunolink.com/articles/wireless/how-do-i-connect-to-a-wireless-network-with-the-esp32/
#include "WiFiConfig.h"
#else
// Option 2
const char *SSID = "Your SSID";
const char *WiFiPassword = "Your Password";
#endif

const uint ServerPort = 23;
WiFiServer Server(ServerPort);
WiFiClient RemoteClient;

ArduinoTimer SendTimer;

/*  Connect to a WiFi network. Assumes the SSID and
 *  password are stored in global variables SSID and
 *  WiFiPassword. For more information see:
 *  https://www.megunolink.com/articles/wireless/how-do-i-connect-to-a-wireless-network-with-the-esp32/
 */
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

/*  Advertise this device on the local network using
 *  mDNS. For more information see:
 */
void AdvertiseServices()
{
  String MyName = MakeMine("MyDevice");
  if (MDNS.begin(MyName.c_str()))
  {
    Serial.println(F("mDNS responder started"));
    Serial.print(F("My name is: "));
    Serial.println(MyName.c_str());

    // Add service to MDNS-SD
    MDNS.addService("n8i-mlp", "tcp", ServerPort);
  }
  else
  {
    Serial.println(F("Error setting up MDNS responder"));
  }
}

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

/*  Looks for and accepts new connections if we aren't
 *  currently connected to a remote client.
 */
void CheckForConnections()
{
  if (Server.hasClient())
  {
    // If we are already connected to another computer, 
    // then reject the new connection. Otherwise accept
    // the connection. 
    if (RemoteClient.connected())
    {
      Serial.println("Connection rejected");
      Server.available().stop();
    }
    else
    {
      Serial.println("Connection accepted");
      RemoteClient = Server.available();
    }
  }
}

/*  Echos any data we receive from a remote client
 *  back to it.
 */
void EchoReceivedData()
{
  uint8_t ReceiveBuffer[30];
  while (RemoteClient.connected() && RemoteClient.available())
  {
    int Received = RemoteClient.read(ReceiveBuffer, sizeof(ReceiveBuffer));
    RemoteClient.write(ReceiveBuffer, Received);
  }
}


void setup()
{
  Serial.begin(9600);
  delay(2000);
  ConnectToWiFi();
  AdvertiseServices();
  Server.begin();
}

void loop()
{
  CheckForConnections();

#if defined(ARDUINO_ARCH_ESP8266)
  MDNS.update();
#endif

  if (SendTimer.TimePassed_Milliseconds(200))
  {
    if (RemoteClient.connected())
    {
      TimePlot MyPlot("", RemoteClient);
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
    }
  }

  EchoReceivedData();
}