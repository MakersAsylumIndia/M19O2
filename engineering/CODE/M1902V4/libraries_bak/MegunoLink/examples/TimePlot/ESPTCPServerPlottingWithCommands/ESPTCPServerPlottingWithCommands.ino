/* ************************************************************************
   Adjust Wifi Plotting Frequency
   This program is a demonstration of the MegunoLink library for sending
   commands to the Arduino over a wireless connection to the ESP32 or
   ESP8266.
   All commands start with a ! character and end with \r (carrige return).
   It responds to the following serial commands:
      !PlottingPeriod #\r\n
        Sets the period (#) between plotting messages in [milliseconds]

   The program folder also contains a MegunoLink project, with an Interface
   Panel to control the parameters.
   Visit:
   * http://www.MegunoLink.com to download MegunoLink.
   * http://www.megunolink.com/documentation/build-arduino-interface/
     for more information on this example.
   ************************************************************************ */
#if defined(ARDUINO_ARCH_ESP32)
#include "WiFi.h"
#include <ESPmDNS.h>
#elif defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#else
#error Not compatible
#endif

// MegunoLink command handler library. If this include fails, you may need to
// install the Arduino library: http://www.megunolink.com/documentation/arduino-integration/
#include "TCPCommandHandler.h"
#include "CommandProcessor.h"
#include "MegunoLink.h"
#include "ArduinoTimer.h"

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

const uint8_t ServerPort = 23;
WiFiServer Server(ServerPort);


ArduinoTimer SendTimer;
uint32_t PlottingPeriod = 200;
TCPCommandHandler<2> Cmds(Server);
CommandProcessor<> SerialCmds(Cmds);


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

void Cmd_ListAll(CommandParameter &Parameters)
{
  Parameters.GetSource().print(F("PlottingPeriod [ms]="));
  Parameters.GetSource().println(PlottingPeriod);
}

void Cmd_SetPlottingPeriod(CommandParameter &Parameters)
{
  PlottingPeriod = Parameters.NextParameterAsInteger(PlottingPeriod);
}

void Cmd_Unknown()
{
  Serial.println(F("I don't understand"));
}



// the setup function runs once when you press reset or power the board
void setup()
{
  Serial.begin(9600);
  Serial.println("TCP Command and Plotting Example");
  ConnectToWiFi();

  AdvertiseServices();

  // Start the TCP server
  Server.begin();
  Server.setNoDelay(true);

  // Setup the serial commands we can repond to
  Cmds.AddCommand(F("PlottingPeriod"), Cmd_SetPlottingPeriod);
  Cmds.AddCommand(F("ListAll"), Cmd_ListAll);
  Cmds.SetDefaultHandler(Cmd_Unknown);

  Serial.println("Ready.");
}

// the loop function runs over and over again until power down or reset
void loop()
{
  MDNS.update();

  // Check for serial commands and dispatch them.
  Cmds.Process();
  SerialCmds.Process();


  if (SendTimer.TimePassed_Milliseconds(PlottingPeriod))
  {
      Serial.println("~");
      TimePlot MyPlot("", Cmds); //Needs to use Cmds to access the connections
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
