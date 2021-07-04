/* SCHEMATIC****************
   I/O pins:
   DHT       - D2  // Temp/rH sensor
   Buzzer    - D3  // Direct to GPIO
   BUTTON    - D4  // Turns compressor ON/OFF
   SIEVE A   - D5  // Relay #1
   SIEVE B   - D6  // Relay #2
   RELAY 3   - D7  // Relay #3 NOT CONNECTED
   RELAY 4   - D8  // Relay #4 Compressor
   RGB LED   - Common Anode, to 3V3 without resistor
               D9  // RED
               D10 // GREEN
               D11 // BLUE
   I2C         A4  // SDA
   I2C         A5  // SCL
   OLED      - I2C ADDR 0x3C // SSD1305 controller, 2.23" OLED HAT 128*32 resolution
   O2 Sensor - I2C ADDR 0x48 // OOM102, with 3 pin header + ADS1115 ADC
*/

//Libraries
//****************************************************************
#include <EEPROM.h>
#include <DHT.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1305.h>
#include <Adafruit_ADS1X15.h>
#include "uptime.h"
#include "uptime_formatter.h"

// I2C DISPLAY
//****************************************************************
#define OLED_RESET -1        //ARDUINO RESET PIN
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1305 display(128, 32, &Wire, OLED_RESET);

//DHT PARAMETERS
//****************************************************************
#define DHTPIN 2
#define DHTTYPE DHT22       // DHT 22
DHT dht(DHTPIN, DHTTYPE);

//ADC PARAMETERS FOR O2 CONCENTRATION
//****************************************************************
Adafruit_ADS1115 ads;
int adc0;                           //sensor connected on adc 0 pin 
float o2percent;                    //variable to store o2 concentration
float factor;                       //multiplication factor to convert adc values in O2 concentration 
const float multiplier = 0.1875F;   //ADC 1 bit = 0.1875mV , ADC set to default gain

//DIGITAL PIN ASSIGNMENTS
//****************************************************************
#define RED 9                       //RED LED
#define GREEN 10                    //GREEN LED
#define BLUE 11                     //BLUE LED
#define BUZZER 3                    //BUZZER PIN
#define BUTTON_PIN 4                //BUTTON PIN FOR CONTACTOR RELAY
#define SIEVE_A_PIN 5               //SIEVE A PIN
#define SIEVE_B_PIN 6               //SIEVE B PIN
#define COMP_RELAY 8                //COMP RELAY

//VARIABLES
//****************************************************************
//Time update parameters
void RUN_CYCLE();
void UPDATE_TIME();

int currentTimeSeconds;
int noOfhours;
int noOfDays;
bool DaysWritten;
 
float h;                    //humidity
float t;                    //temperature

unsigned long prevCycleTime = millis();
int prevCycle = 4;
unsigned long waitTime = 0;

int state = HIGH;           // the current state of the output pin
int reading;                // the current reading from the input pin
int previous = LOW;         //
unsigned long time = 0;     // the last time the output pin was toggled
unsigned long debounce = 200UL;

//Set delay timing here
//**************************************************************************
int CYCLE_TIME = 4000;
int FLUSH_TIME = 850;
int PRODUCTION_TIME = CYCLE_TIME - 2 * FLUSH_TIME;

//**************************************************************************

void setup() {
  Serial.begin(9600);       //serial communication for debugging
  Serial.println("Starting the machine");
//  dht.begin();              //start DHT communication
//  EEPROM.begin();

//check the display
//  if ( ! display.begin(0x3C) ) {
//    Serial.println("Unable to initialize OLED");
//    while (1) yield();
//  }
//  display.clearDisplay();   // clears the screen and buffer
//  delay(1000);
//  display.clearDisplay();

//Get time from the memory
//**************************************************************************
//  noOfDays = EEPROM.read(5);
//  noOfhours = EEPROM.read(1);

// SET PIN MODE FOR PINS IN PROGRAM
//**************************************************************************
  pinMode(SIEVE_A_PIN, OUTPUT);
  pinMode(SIEVE_B_PIN, OUTPUT);

//define pins as input &output
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(COMP_RELAY, OUTPUT);

//Set initial states here
//**************************************************************************
//set SIEVE pins to LOW(turn off the relays)
  digitalWrite(SIEVE_A_PIN, HIGH);
  digitalWrite(SIEVE_B_PIN, HIGH);

//Serial.println("Sieve A and B: OFF");

//Set LED pins to LOW(turn off the LEDs)

  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, HIGH);

//ADC values
  int adc_ambAirValue = ads.readADC_SingleEnded(0);  //adc value in ambient air
  factor = adc_ambAirValue/20.9;                     //calculate the factor 
  float adc_maxValue = factor*100;                   //update max value of adc @98.6% o2 conc.(known)
  Serial.print(adc_ambAirValue);
  Serial.print('\t');
  Serial.print(factor);
  Serial.print('\t');
  Serial.println(adc_maxValue);
}

void loop() {
  //UPDATE_TIME();       //update run time
  //COMP_SWITCH();       //turn on/off the compressor
  RUN_CYCLE();         //activate the sieves
  //GET_TEMP();          //get the temperature and humidity
  //ALARM(h);            //set led colors and trigger alarm
  //O2_CONCENTRATION();
  //DISPLAY_DATA();      //display readings
}

void RUN_CYCLE() {
//CYCLE 1
//*****************************************************
  if ((millis() - prevCycleTime >= waitTime) && (prevCycle == 4)) {
    Serial.println("Cycle 1");
    digitalWrite(SIEVE_A_PIN, LOW);
    digitalWrite(SIEVE_B_PIN, LOW);
    Serial.println("SEIVE A: ON, SEIVE B: ON");
    prevCycleTime = millis();
    prevCycle = 1;
    waitTime = FLUSH_TIME;
    //Serial.println(FLUSH_TIME);
  }

//CYCLE2
//****************************************************
  if ((millis() - prevCycleTime >= waitTime) && (prevCycle == 1)) {
    Serial.println("Cycle 2");
    digitalWrite(SIEVE_B_PIN, HIGH);
    Serial.println("SEIVE A: ON, SEIVE B: OFF");
    prevCycleTime = millis();
    prevCycle = 2;
    waitTime = PRODUCTION_TIME;
    //Serial.println(PRODUCTION_TIME);
  }

//CYCLE 3
//****************************************************
  if ((millis() - prevCycleTime >= waitTime) && (prevCycle == 2)) {
    Serial.println("Cycle 3");
    digitalWrite(SIEVE_B_PIN, LOW);
    Serial.println("SEIVE A: ON, SEIVER B: ON");
    prevCycleTime = millis();
    prevCycle = 3;
    waitTime = FLUSH_TIME;
    //Serial.println(FLUSH_TIME);
  }

//CYCLE 4
//***************************************************
  if ((millis() - prevCycleTime >= waitTime) && (prevCycle == 3)) {
    Serial.println("Cycle 4");
    digitalWrite(SIEVE_A_PIN, HIGH);
    Serial.println("SEIVE A: OFF, SEIVE B: ON");
    prevCycleTime = millis();
    prevCycle = 4;
    waitTime = PRODUCTION_TIME;
    //Serial.println(PRODUCTION_TIME);
    Serial.println("");
  }


}
void UPDATE_TIME() {
  DaysWritten = false;
  uptime::calculateUptime();
  Serial.println("update loop");
  if (uptime::getHours() != currentTimeSeconds) {
    //Serial.println("time loop 1");
    noOfhours += 1;
    EEPROM.write(1, noOfhours);
    currentTimeSeconds = uptime::getHours();
    Serial.println(noOfhours);

    Serial.print(uptime::getSeconds());
    Serial.println('s');
    Serial.print(uptime::getMinutes());
    Serial.println('m');
    Serial.print(uptime::getHours());
    Serial.println('h');
    Serial.print(uptime::getDays());
    Serial.println('d');

    if (noOfhours == 24 && DaysWritten == false) {
      noOfDays += 1;
      EEPROM.write(5, noOfDays);
      Serial.println("no of days = ");
      Serial.println(noOfDays);
      noOfhours = 0;
      DaysWritten = true;
    }
  }
}

void GET_TEMP() {
  h = dht.readHumidity();
  t = dht.readTemperature();
  Serial.println("DHT Loop");
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);
    tone(BUZZER, 1200);
    return;
  }
//  Serial.println("Got the tem and humidity readings");
//  Serial.print("Temperature: ");
//  Serial.println(t);
//  Serial.print("Humidity: ");
//  Serial.println(h);

}

void ALARM(int h) {
  Serial.println("Alarm loop");
  if (h>0 && h <= 15) {                    //Green color if humidity is less than 15
    analogWrite(RED, 255);          //turn off red
    analogWrite(GREEN, 180);        //turn on Green
    //Serial.println("less than 15");
    //Serial.println("Green ON");
    noTone(BUZZER);
  }

  else if (h > 15 && h <= 30) {     //blink green led if humidity is from 15 to 30 percent
    analogWrite(RED, 255);          //turn off red
    //Serial.println("less than 30");
    analogWrite(GREEN, 180);        //turn on green
    analogWrite(GREEN, 255);        //turn off green
    //Serial.println("Green blinking");
    noTone(BUZZER);
  }

  else if (h > 30 && h <= 50) {     //turn on red led if humidity is from 30 to 50 percent
//  Serial.println("less than 50");   //turn off green
    analogWrite(GREEN, 255);
    analogWrite(RED, 180);          //turn on red
    noTone(BUZZER);
    //Serial.println("Red ON");
    noTone(BUZZER);
  }

  else if (h > 50) {                //blink red led and trigger the alarm
    //Serial.println("more than 50");
    analogWrite(GREEN, 255);        //turn off green
    //Serial.println("Red blinking");
    //Serial.println("Alarm");
    tone(BUZZER, 500);
    analogWrite(RED, 150);
    tone(BUZZER, 250);
    analogWrite(RED, 200);

  }

  else {
    analogWrite(RED, 255);
    analogWrite(GREEN, 255);
    analogWrite(BLUE, 255);
    noTone(BUZZER);
  }

}

void COMP_SWITCH() {
  reading = digitalRead(BUTTON_PIN);
  Serial.println("Button loop ");
  //Serial.println(reading);

  if (reading == HIGH && previous == LOW && millis() - time > debounce) {
    if (state == HIGH) {
      state = LOW;
    }
    else {
      state = HIGH;
    }
    time = millis();
  }

  digitalWrite(COMP_RELAY, state);
  //Serial.print("Compressor: ");
  //Serial.println(state);

  previous = reading;
}

void DISPLAY_DATA() {
  Serial.println("display loop");
  display.clearDisplay();             //clear the display
  display.setTextSize(1);             //set text siz, 1 is 10 pixel height
  display.setTextColor(WHITE);        //set color
  display.setCursor(0, 0);
  display.println("TEMP: ");          //print Temp:

  display.setCursor(30, 0);
  display.println(t, 0);              //print the temperature value, without decimal points

  display.setCursor(70, 0);
  display.println("RH: ");            //print Hum :

  display.setCursor(100, 0);
  display.println(h, 0);              //print the humidity value, without decimal points

  display.setCursor(0, 12);
  display.println("O2% : ");          // print DP :

  display.setCursor(30, 12);
  if(o2percent<100){
  display.println(o2percent, 0);
  }
  else{
    display.println("ERROR");
  }
    
  display.setCursor(70, 12);
  display.println("COMP: ");

  display.setCursor(100, 12);
  if (state == HIGH) {
    display.println("OFF");           //relays logic is reversed
  } else {
    display.println("ON");            //relays logic is reversed
  }

  display.display();                  //display data
}

void O2_CONCENTRATION(){
  Serial.println("O2 loop");
  adc0 = ads.readADC_SingleEnded(0); // Read ADC value from ADS1115
  Serial.print("Analog pin 0: ");
  Serial.print(adc0);
  Serial.print('\t');
  o2percent = adc0/factor;
  Serial.println(o2percent);
  if(o2percent>100){
    Serial.println("Error reading oxygen percent");
  }
}
