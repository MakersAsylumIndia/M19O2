/* **********************************************************************************************
 *  Example program which sends MegunoLink coordinates to plot on its Mapping visualiser.
 *  
 *  Visit http://www.megunolink.com/documentation/mapping/ for more information.
 *  
 *  ********************************************************************************************** */
#include "MegunoLink.h"

Map MyMap; 


void setup()
{
  Serial.begin(9600);
}




void loop()
{

  // Sends the microcontroller coordinates for three sensor locations
  MyMap.SendData("Sensor Location 1",-37.785949,175.30081);
  MyMap.SendData("Sensor Location 2","-37.795949","175.30081");
  MyMap.SendData("Sensor Location 3",-37.775949,175.30081);


  delay(1000);
}


