/************************************************************************************************
Example Description
Data is sent via an ESP32 network interface using a TCP connection to MegunoLink where it is
plotted in real time.

More Information
*  http://www.megunolink.com/documentation/plotting/
*  https://www.megunolink.com/articles/wireless/how-do-i-connect-to-a-wireless-network-with-the-esp32/
*  https://www.megunolink.com/articles/wireless/find-esp32-esp8266-ip-address-with-mdns/

This Example Requires:
*  The MegunoLink arduino library https://www.megunolink.com/documentation/arduino-integration/

MegunoLink Interface
You can download a pre-made interface from here:
https://github.com/Megunolink/MLP/raw/master/examples/TimePlot/ESP32TCPWirelessPlotting/ESP32TCPWirelessPlotting.mlpz

You can find out more about MegunoLink and download a free trial from here
https://www.megunolink.com/
https://www.megunolink.com/download/
************************************************************************************************/

#include "WiFi.h"
#include "ESPmDNS.h"

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
 *
 */
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

void SendHallValue()
{
  if (RemoteClient.connected())
  {
    TimePlot HallPlot("", RemoteClient);
    HallPlot.SendData("Magnetic Field", hallRead());
  }
}

void setup()
{
  Serial.begin(9600);
  ConnectToWiFi();
  AdvertiseServices("HallSensor");
  Server.begin();
}

void loop()
{
  CheckForConnections();

  if (SendTimer.TimePassed_Milliseconds(400))
  {
    SendHallValue();
  }

  EchoReceivedData();
}