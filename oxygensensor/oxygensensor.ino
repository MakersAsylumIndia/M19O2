//Libraries
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include<Filter.h>

//sensor parameters
Adafruit_ADS1115 ads;
int adc0;
float o2percent;

//calibrated values
//**************************************************************
const float multiplier =0.0625;
int adc_ambAirValue;
float factor;
int adc_maxValue ;

void setup()
{
  Serial.begin(9600);     //serial communication for debugging
  ads.begin();
  adc_ambAirValue = ads.readADC_SingleEnded(0);
  factor = adc_ambAirValue / 20.9;
  adc_maxValue = factor * 100;
}

void loop()
{

  adc0 = ads.readADC_SingleEnded(0); // Read ADC value from ADS1115
  Serial.print("Analog pin 0: ");
  Serial.println(adc0);
  //  Serial.print('\t');
  //  o2percent = adc0/factor;
  //  Serial.println(o2percent);
  //  if(o2percent>100){
  //    Serial.println("Error reading oxygen percent");
  //  }
  delay(500);
}
