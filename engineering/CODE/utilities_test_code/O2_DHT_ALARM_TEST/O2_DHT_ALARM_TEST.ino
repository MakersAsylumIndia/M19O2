/* SCHEMATIC****************
   Relay pins:
   SIEVE A- D5
   SIEVE B- D6
   RELAY 3- D7 //NOT CONNECTED
   RELAY 4- D8 //For the compressor relay
   BUTTON - D4 //To turn on/off the compressor
   DHT    - D2
   Buzzer - D3

   RGB LED- D9: RED, D10: GREEN, D11: BLUE
   OLED   - DIN(SDA): A4, SCL: A5
*/

//Libraries
//****************************************************************
#include <DHT.h>
#include <SPI.h>
#include <Wire.h>
#include <Filter.h>                      //  https://www.megunolink.com/documentation/arduino-libraries/exponential-filter/
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1305.h>
#include <Adafruit_ADS1015.h>

// I2C DISPLAY
//****************************************************************
#define OLED_RESET -1          //ARDUINO RESET PIN
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1305 display(128, 32, &Wire, OLED_RESET);


//DHT PARAMETERS
//****************************************************************
#define DHTPIN 2
#define DHTTYPE DHT22       // DHT 22
DHT dht(DHTPIN, DHTTYPE);


//ADC PARAMETERS FOR O2 CONCNTRATION
//****************************************************************
Adafruit_ADS1115 ads;
int adc0;                           //sensor connected on adc 0 pin
int O2;                           //variable to store o2 concentration
float factor;                       //multiplication factor to convert adc values in O2 concentration
const float multiplier = 0.1875F;   //ADC 1 bit = 0.1875mV , ADC set to default gain
float O2_SMOOTH;                    //Filtered value
ExponentialFilter<float> O2_FILTERED(20, 20.9);   // the <float> makes a filter for float numbers // 20 is the weight (20 => 20%) // 20.9 is the initial value of the filter


//DIGITAL PIN ASSIGNMENTS
//****************************************************************
#define RED 9                       //RED LED
#define GREEN 10                    //GREEN LED
#define BLUE 11                     //BLUE LED

//VARIABLES
//****************************************************************
float h;                    //humidity
float t;                    //temperature


//**************************************************************************
void setup() {
  Serial.begin(9600);       //serial communication for debugging
  Serial.println("Starting the machine");
  dht.begin();              //start DHT communication

  //check the display
  if ( ! display.begin(0x3C) ) {
    Serial.println("Unable to initialize OLED");
    while (1) yield();

  }
  display.clearDisplay();   // clears the screen and buffer
  delay(1000);
  display.clearDisplay();

  //define pins as input &output
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  //Set LED pins to LOW(turn off the LEDs)
  //**************************************************************************
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, HIGH);

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

void loop() {
  GET_TEMP();          //get the temperature and humidity
  ALARM(h);            //set led colors and trigger alarm
  O2_CONCENTRATION();  //Calculate o2 concentration
  DISPLAY_DATA();      //display readings
}

//GET TEMPERATURE FROM THE DHT SENSOR
//**************************************************************************
void GET_TEMP() {
  h = dht.readHumidity();
  t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);
    return;
  }
}
//DISPLAY READINGS ON THE SCREEN
//**************************************************************************
void DISPLAY_DATA() {
  display.clearDisplay();             //clear the display
  display.setTextSize(1);             //set text size, 1 is 10 pixel height
  display.setTextColor(WHITE);        //set color
  display.setCursor(0, 0);
  display.println("TEMP: ");          //print Temp:

  display.setCursor(30, 0);
  display.println(t, 0);              //print the temperature value, without decimal points
  display.setCursor(45, 0);
  if (t < 30) {
    display.println("norm");
  } else if (t > 30 && t <= 40) {
    display.println("chk");
  } else if (t > 40 && t <= 45) {
    display.println("high");
  } else if (t > 45) {
    display.println("warn");
  }

  display.setCursor(80, 0);
  display.println("HUM: ");            //print Hum :

  display.setCursor(105, 0);
  display.println(h, 0);              //print the humidity value, without decimal points

  display.setCursor(0, 12);
  display.println("O2% : ");

  display.setCursor(30, 12);
  if (O2_SMOOTH <= 100) {
    display.println(O2_SMOOTH, 0);
  }
  else {
    display.println("ERROR");
  }

  display.display();                  //display data
}


//CALCULATE THE O2 CONCENTRATION
//**************************************************************************
void O2_CONCENTRATION() {
  adc0 = ads.readADC_SingleEnded(0); // Read ADC value from ADS1115
//  Serial.println(factor);
//  Serial.print("Analog pin 0: ");
//  Serial.print(adc0);
  O2 = adc0 / factor;
  O2_FILTERED.Filter(O2);            //Calculate the filtered value
  O2_SMOOTH = O2_FILTERED.Current(); //calculate current filtered value
  
//  Serial.print("\t");
//  Serial.print(O2);
//  Serial.print("\t");
//
//  Serial.println(O2_SMOOTH);
  if (O2_SMOOTH > 100 && O2_SMOOTH < 20) {
    Serial.println("Error reading oxygen percent");
  }
}


//CHANGE LED COLOR BASED ON O2 CONCENTRATION
//**************************************************************************
void ALARM(int h) {                 // turn on red if o2 conc. is less than 80%, blue for 80-90%, green for more than 90%
  if (O2_SMOOTH < 80) {
    analogWrite(RED, 180);          //common anode, 255 to turn off and 0 to turn on
    analogWrite(BLUE, 255);
    analogWrite(GREEN, 255);
  }

  else if (O2_SMOOTH >= 80 && O2_SMOOTH <= 90) {
    analogWrite(RED, 255);
    analogWrite(BLUE, 180);
    analogWrite(GREEN, 255);
  }

  else if (O2_SMOOTH > 90) {
    analogWrite(GREEN, 180);
    analogWrite(BLUE, 255);
    analogWrite(RED, 255);
  }

  else {
    analogWrite(RED, 255);
    analogWrite(GREEN, 255);
    analogWrite(BLUE, 255);
  }
}
