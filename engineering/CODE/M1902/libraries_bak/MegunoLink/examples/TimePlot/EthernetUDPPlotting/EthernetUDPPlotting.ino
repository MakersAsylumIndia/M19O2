/************************************************************************************************
Example Description
Data is sent from a Wiznet 5100 ethernet adapter over UDP to MegunoLink's Time Plot visualiser 
where it is plotted in realtime.

This example uses the raw plotting protocol. Detail about this procotol can be found here:
https://www.megunolink.com/documentation/plotting/plotting-message-reference/
 
More Information
*  http://www.megunolink.com/documentation/plotting/
 
MegunoLink Interface
You can download a pre-made interface from here:
https://github.com/Megunolink/MLP/raw/master/examples/TimePlot/EthernetUDPlotting/EthernetUDPPlotting.mlpz
 
You can find out more about MegunoLink and download a free trial from here
https://www.megunolink.com/
https://www.megunolink.com/download/
************************************************************************************************/


#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h>         // UDP library from: bjoern@cs.stanford.edu 12/30/2008

// Enter a MAC address
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  
//Local port to listen to (set this in megunolink)  
unsigned int localPort = 1269;

//Local IP address of the computer you want to send data to (255.255.255.255 = broadcast address)
const char localIPAddress[] = "255.255.255.255";

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

// Timer Variables
unsigned long LastPlotTime = 0;




void setup() 
{
  Serial.begin(9600);
  Serial.println("Setup Started");
  
  // start the Ethernet and UDP:
  Ethernet.begin(mac); //IP address will be found automatically
  Udp.begin(localPort);

  Serial.println("Setup Complete");
}

void loop() 
{
  int adcval = analogRead(0); //Read adc value


  //Plot Data Every 1000ms
  if((millis()-LastPlotTime)>1000)
  {
    Serial.println("sending udp data"); //notify user using normal serial port
    LastPlotTime = millis();

    //Send UDP data
    Udp.beginPacket(localIPAddress, localPort);
    Udp.write("{TIMEPLOT|data|adc vale|T|");
    Udp.print(adcval);
    Udp.write("}\n");
    Udp.endPacket();
    
    
  }
}

















