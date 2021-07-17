//Libraries
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>

//sensor parameters
Adafruit_ADS1115 ads;
int adc0;                           //sensor connected on adc 0 pin
int O2;                           //variable to store o2 concentration
float factor;                       //multiplication factor to convert adc values in O2 concentration
const float multiplier = 0.1875F;   //ADC 1 bit = 0.1875mV , ADC set to default gain
float O2_SMOOTH;                    //Filtered value
ExponentialFilter<float> O2_FILTERED(20, 20.9);   // the <float> makes a filter for float numbers // 20 is the weight (20 => 20%) // 20.9 is the initial value of the filter

void setup()
{
  Serial.begin(9600);     //serial communication for debugging
  ads.begin();

  //ADC values
  int adc_ambAirValue = ads.readADC_SingleEnded(0);  //adc value in ambient air
  factor = adc_ambAirValue / 20.9;                   //slope 78.9 - 20.9% - 14.8mV ::: 372.26 - 98.6% - 69.8mv
  float adc_maxValue = 100 * factor;
  Serial.print(adc_ambAirValue);
  Serial.print('\t');
  Serial.print(factor);
  Serial.print('\t');
  Serial.println(adc_maxValue);
  }

void loop()
{
  
  adc0 = ads.readADC_SingleEnded(0); // Read ADC value from ADS1115
  Serial.println(factor);
  Serial.print("Analog pin 0: ");
  Serial.print(adc0);
  
  O2 = adc0 / factor;
  O2_FILTERED.Filter(O2);            //Calculate the filtered value
  O2_SMOOTH = O2_FILTERED.Current(); //calculate current filtered value
  
  Serial.print("\t");
  Serial.print(O2);
  Serial.print("\t");

  Serial.println(O2_SMOOTH);
  if (O2_SMOOTH > 100 && O2_SMOOTH < 20) {
    Serial.println("Error reading oxygen percent");
  }
  delay(500);
}
