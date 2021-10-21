/************************************************************************************************
Example Description
In this example we process commands sent from MegunoLink's Interface Panel using a UDP connection.
 
More Information
*  https://www.megunolink.com/documentation/interface-panel/
 
This Example Requires:
*  The MegunoLink arduino library https://www.megunolink.com/documentation/arduino-integration/
*  The Pstring Library http://arduiniana.org/libraries/pstring/
*  The EtherCard Library https://github.com/njh/EtherCard. 
 
MegunoLink Interface
You can download a pre-made interface from here:
https://github.com/Megunolink/MLP/raw/master/examples/CommandHandler/ENC28J60UDPCommandHandler/ENC28J60UDPCommandHandler.mlpz
 
You can find out more about MegunoLink and download a free trial from here
https://www.megunolink.com/
https://www.megunolink.com/download/
************************************************************************************************/

#include <EtherCard.h>
#include <IPAddress.h>
#include <MegunoLink.h>
#include <CommandDispatcher.h>
#include <PString.h>


// ethernet mac address - must be unique on your network
static byte mymac[] = { 0x70,0x69,0x69,0x2D,0x30,0x31 };
const static uint8_t dstIP[] = { 255,255,255,255 }; //Broadcast address. You can replace this with a specific machines IP
const int dstPort PROGMEM = 1234; //Port on the computer that its talking to
const int srcPort PROGMEM = 4321; //Port on this device that it listens on

byte Ethernet::buffer[500]; // tcp/ip send and receive buffer

// MegunoLink Command Handling
const int MaxCommands = 10;
CommandDispatcher<MaxCommands> Commands;

// Callback that prints received packets to the serial port
void udpSerialPrint(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *data, uint16_t len) {
  IPAddress src(src_ip[0], src_ip[1], src_ip[2], src_ip[3]);

  // Print message details
  Serial.print("dest_port: ");
  Serial.println(dest_port);
  Serial.print("src_port: ");
  Serial.println(src_port);
  Serial.print("src_IP: ");
  ether.printIp(src_ip);
  Serial.println("data: ");
  Serial.println(data);

  // Process Command
  TruncateAtNewLine((char*)data);
  Commands.DispatchCommand((char*)data + 1, Serial); //+1 skips the start of line character "!"
}

void Cmd_GetADCValue(CommandParameter &p)
{
  uint16_t ADCValue = analogRead(0);

  // Update the value shown in table on the example interface panel
  char MessageBuffer[100];
  PString Message(MessageBuffer, sizeof(MessageBuffer));
  Table MLPTable("", Message);
  MLPTable.SendData("A0", ADCValue);
  ether.sendUdp(MessageBuffer, Message.length(), srcPort, dstIP, dstPort);

  // Update the text box on the example interface panel
  Message.begin();
  InterfacePanel MyPanel("", Message);
  MyPanel.SetText("adcTextBox", ADCValue);
  ether.sendUdp(MessageBuffer, Message.length(), srcPort, dstIP, dstPort);
}

void Cmd_Hello(CommandParameter &p)
{
  char textToSend[] = "Hello World!";
  ether.sendUdp(textToSend, sizeof(textToSend) - 1, srcPort, dstIP, dstPort); //-1 removes null terminator
}

// Replaces the first '\r' or '\n' character with a null terminating the string. 
void TruncateAtNewLine(char *Buffer)
{
  for (char *Current = Buffer; *Current; ++Current)
  {
    if (*Current == '\r' || *Current == '\n')
    {
      *Current = '\0';
      break;
    }
  }
}

void setup() {
  Serial.begin(9600);

  // Change 'SS' to your Slave Select pin, if you arn't using the default pin
  if (ether.begin(sizeof Ethernet::buffer, mymac, SS) == 0)
    Serial.println(F("Failed to access Ethernet controller"));

  if (!ether.dhcpSetup())
    Serial.println(F("DHCP failed"));


  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);
  ether.printIp("DNS: ", ether.dnsip);

  //register udpSerialPrint() to source port (see srcPort above)
  ether.udpServerListenOnPort(&udpSerialPrint, srcPort);

  //Add commands to look out for
  Commands.AddCommand(F("Hello"), Cmd_Hello);
  Commands.AddCommand(F("GetADCValue"), Cmd_GetADCValue);
}

void loop() {
  //this must be called for ethercard functions to work.
  ether.packetLoop(ether.packetReceive());
}
