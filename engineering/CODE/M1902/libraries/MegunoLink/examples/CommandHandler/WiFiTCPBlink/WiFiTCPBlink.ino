/* ************************************************************************
   BlinkWiFi
   This program is a demonstration of the MegunoLink library for sending
   commands to the Arduino over a wireless connection to the ESP32 or 
   ESP8266. 

   All commands start with a ! character and end with \r (carrige return). 
   It responds to the following serial commands:
      !OnTime n\r\n
        Sets the amount of time the LED remains on to n [milliseconds]

      !OnTime ?\r\n
        Returns the amount of time the LED remains on [milliseconds]

      !OffTime n\r\n
        Sets the amount of time the LED remains off to n [milliseconds]

      !OffTime ?\r\n
        Returns the amount of time the LED remains off [milliseconds]

   The program folder also contains a MegunoLink project, with an Interface
   Panel to control the parameters.
   Visit:
   * http://www.MegunoLink.com to download MegunoLink.
   * http://www.megunolink.com/documentation/build-arduino-interface/
     for more information on this example.
   * Check out our video to see this example in action 
     https://www.youtube.com/watch?v=sbVoMVBGnIk
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

TCPCommandHandler<2> Cmds(Server);
CommandProcessor<> SerialCmds(Cmds);

long LastBlink = 0; // Time we last blinked the LED
int OnTime = 10; // Amount of time the LED remains on [milliseconds]
int OffTime = 100; // Amount of time the LED remains off [milliseconds]

#if defined(ARDUINO_ARCH_ESP32)
const int LEDPin = 23; // Pin the LED is attached to 
#elif defined(ARDUINO_ARCH_ESP8266)
const int LEDPin = BUILTIN_LED; // Pin the LED is attached to 
#endif


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
  Parameters.GetSource().print(F("OnTime [ms]="));
  Parameters.GetSource().println(OnTime);
  Parameters.GetSource().print(F("OffTime [ms]="));
  Parameters.GetSource().println(OffTime);
}

void Cmd_SendToAll(CommandParameter &Parameters)
{
  Cmds.print(F("OnTime [ms]="));
  Cmds.println(OnTime);
  Cmds.print(F("OffTime [ms]="));
  Cmds.println(OffTime);
}

void Cmd_SetOnTime(CommandParameter &Parameters)
{
  OnTime = Parameters.NextParameterAsInteger(OnTime);
}

void Cmd_SetOffTime(CommandParameter &Parameters)
{
  OffTime = Parameters.NextParameterAsInteger(OffTime);
}

void Cmd_Unknown()
{
  Serial.println(F("I don't understand"));
}

// the setup function runs once when you press reset or power the board
void setup()
{
  Serial.begin(9600);
  Serial.println("TCP Command Example");
  ConnectToWiFi();

  AdvertiseServices();

  // Start the TCP server
  Server.begin();
  Server.setNoDelay(true);

  // Setup the serial commands we can repond to
  Cmds.AddCommand(F("OnTime"), Cmd_SetOnTime);
  Cmds.AddCommand(F("OffTime"), Cmd_SetOffTime);
  Cmds.AddCommand(F("ListAll"), Cmd_ListAll);
  Cmds.AddCommand(F("SendToAll"), Cmd_SendToAll);
  Cmds.SetDefaultHandler(Cmd_Unknown);

  pinMode(LEDPin, OUTPUT);

  Serial.println("Ready.");
}

// the loop function runs over and over again until power down or reset
void loop() 
{
  MDNS.update();
  
  // Check for serial commands and dispatch them.
  Cmds.Process();
  SerialCmds.Process();

  // Update the LED
  uint32_t uNow = millis();
  if (uNow - LastBlink < OnTime)
  {
    digitalWrite(LEDPin, HIGH);
  }
  else
  {
    digitalWrite(LEDPin, LOW);
  }

  if (uNow - LastBlink > OnTime + OffTime)
  {
    LastBlink = uNow;
  }

}
