/* SCHEMATIC****************
   Relay pins:
   SIEVE A- D5
   SIEVE B- D6
   EQ_VALVE- D7 //NOT CONNECTED
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
const float multiplier = 0.1875;   //ADC 1 bit = 0.1875mV , ADC set to default gain
float O2_SMOOTH;                    //Filtered value
ExponentialFilter<float> O2_FILTERED(20, 20.9);   // the <float> makes a filter for float numbers // 20 is the weight (20 => 20%) // 20.9 is the initial value of the filter


//DIGITAL PIN ASSIGNMENTS
//****************************************************************
#define RED 9                       //RED LED
#define GREEN 10                    //GREEN LED
#define BLUE 11                     //BLUE LED
#define BUZZER 3                    //BUZZER PIN
#define BUTTON_PIN 4                //BUTTON PIN FOR CONTACTOR RELAY
#define SIEVE_A_PIN 5               //SIEVE A PIN
#define SIEVE_B_PIN 6               //SIEVE B PIN
#define EQ_VALVE_PIN 7             //EQULIZATION VALVE PIN
#define COMP_RELAY 8                //COMP RELAY


//VARIABLES
//****************************************************************
int h;                    //humidity
int t;                    //temperature

unsigned long prevCycleTime = millis();
int     prevCycle = 8;
unsigned long waitTime = 0;

bool state = HIGH;           // the current state of the output pin
int reading;                 // the current reading from the input pin
bool previous = LOW;
unsigned long Time = 0;      // the last time the output pin was toggled
const int debounce = 200;


//Set delay timing here
//**************************************************************************
const int S_ON_Eq_OFF = 6000;     //One sieve on, and equalisation valve off
const int S_ON_Eq_ON = 200;       //One sieve on, and euqialisation valve on
const int S_OFF_Eq_ON = 800;      //sieves off, equlaization vale on
const int S_OFF_Eq_OFF = 200;     //all valves low


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

  // SET PIN MODE FOR PINS IN PROGRAM
  //**************************************************************************
  pinMode(SIEVE_A_PIN, OUTPUT);
  pinMode(SIEVE_B_PIN, OUTPUT);
  pinMode(EQ_VALVE_PIN, OUTPUT);

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
  digitalWrite(EQ_VALVE_PIN, HIGH);

  //Set LED pins to LOW(turn off the LEDs)
  //**************************************************************************
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, HIGH);

  //ADC values
  int adc_ambAirValue = ads.readADC_SingleEnded(0);  //adc value in ambient air
  if (adc_ambAirValue > 85) {
    factor = 3.82;
  } else {
    factor = adc_ambAirValue / 20.9;                   //slope 78.9 - 20.9% - 14.8mV ::: 372.26 - 98.6% - 69.8mv
  } 
  //Serial.println(adc_ambAirValue);
  //  float adc_maxValue = 100 * factor;
  //Serial.println(adc_ambAirValue);
  //  Serial.print('\t');
  //  Serial.print(factor);
  //  Serial.print('\t');
  //  Serial.println(adc_maxValue);
}

void loop() {
  COMP_SWITCH();       //turn on/off the compressor
  RUN_CYCLE();         //activate the sieves
  GET_TEMP();          //get the temperature and humidity
  //ALARM(h);            //set led colors and trigger alarm
  O2_CONCENTRATION();  //Calculate o2 concentration
  DISPLAY_DATA();      //display readings
}

void RUN_CYCLE() {
  //CYCLE 1
  //*****************************************************
  if ((millis() - prevCycleTime >= waitTime) && (prevCycle == 8)) {
    //Serial.println("Cycle 1");
    digitalWrite(SIEVE_A_PIN, LOW);
    digitalWrite(SIEVE_B_PIN, HIGH);
    digitalWrite(EQ_VALVE_PIN, HIGH);
    Serial.println("SEIVE A: ON, SEIVE B: OFF, EQ : OFF");
    prevCycleTime = millis();
    prevCycle = 1;
    waitTime = S_ON_Eq_OFF;
  }

  //CYCLE2
  //****************************************************
  if ((millis() - prevCycleTime >= waitTime) && (prevCycle == 1)) {
    //Serial.println("Cycle 2");
    digitalWrite(EQ_VALVE_PIN, LOW);
    Serial.println("SEIVE A: ON, SEIVE B: OFF, EQ : ON");
    prevCycleTime = millis();
    prevCycle = 2;
    waitTime = S_ON_Eq_ON;
  }

  //CYCLE 3
  //****************************************************
  if ((millis() - prevCycleTime >= waitTime) && (prevCycle == 2)) {
    //Serial.println("Cycle 3");
    digitalWrite(SIEVE_A_PIN, HIGH);
    Serial.println("SEIVE A: OFF, SEIVER B: OFF, EQ : ON");
    prevCycleTime = millis();
    prevCycle = 3;
    waitTime = S_OFF_Eq_ON;
  }

  //CYCLE 4
  //***************************************************
  if ((millis() - prevCycleTime >= waitTime) && (prevCycle == 3)) {
    //Serial.println("Cycle 4");
    digitalWrite(EQ_VALVE_PIN, HIGH);
    Serial.println("SEIVE A: OFF, SEIVE B: OFF, EQ: OFF");
    prevCycleTime = millis();
    prevCycle = 4;
    waitTime = S_OFF_Eq_OFF;
  }

  //CYCLE 5
  //*****************************************************
  if ((millis() - prevCycleTime >= waitTime) && (prevCycle == 4)) {
    //Serial.println("Cycle 4");
    digitalWrite(SIEVE_A_PIN, HIGH);
    digitalWrite(SIEVE_B_PIN, LOW);
    digitalWrite(EQ_VALVE_PIN, HIGH);
    Serial.println("SEIVE A: OFF, SEIVE B: ON, EQ : OFF");
    prevCycleTime = millis();
    prevCycle = 5;
    waitTime = S_ON_Eq_OFF;
  }

  //CYCLE6
  //****************************************************
  if ((millis() - prevCycleTime >= waitTime) && (prevCycle == 5)) {
    //Serial.println("Cycle 5");
    digitalWrite(EQ_VALVE_PIN, LOW);
    Serial.println("SEIVE A: OFF, SEIVE B: ON, EQ : ON");
    prevCycleTime = millis();
    prevCycle = 6;
    waitTime = S_ON_Eq_ON;
  }

  //CYCLE 7
  //****************************************************
  if ((millis() - prevCycleTime >= waitTime) && (prevCycle == 6)) {
    //Serial.println("Cycle 3");
    digitalWrite(SIEVE_B_PIN, HIGH);
    Serial.println("SEIVE A: OFF, SEIVER B: OFF, EQ : ON");
    prevCycleTime = millis();
    prevCycle = 7;
    waitTime = S_OFF_Eq_ON;
  }

  //CYCLE 8
  //***************************************************
  if ((millis() - prevCycleTime >= waitTime) && (prevCycle == 7)) {
    //Serial.println("Cycle 4");
    digitalWrite(EQ_VALVE_PIN, HIGH);
    Serial.println("SEIVE A: OFF, SEIVE B: OFF, EQ: OFF");
    prevCycleTime = millis();
    prevCycle = 8;
    waitTime = S_OFF_Eq_OFF;
  }
}

//GET TEMPERATURE FROM THE DHT SENSOR
//**************************************************************************
void GET_TEMP() {
  h = dht.readHumidity();
  t = dht.readTemperature();
  //  Serial.println(h);
  //  Serial.println(t);
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);
    return;
  }
}

//CONTROL THE COMPRESSOR RELAY FROM COMPRESSOR SWITCH
//**************************************************************************
void COMP_SWITCH() {
  reading = digitalRead(BUTTON_PIN);

  if (reading == HIGH && previous == LOW && millis() - Time > debounce) {
    if (state == HIGH) {
      state = LOW;
    }
    else {
      state = HIGH;
    }
    Time = millis();
  }

  digitalWrite(COMP_RELAY, state);
  previous = reading;
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

  display.setCursor(70, 12);
  display.println("COMP: ");

  display.setCursor(100, 12);
  if (state == HIGH) {
    display.println("OFF");           //relays have reversed logic
  } else {
    display.println("ON");            //relays have reversed logic
  }

  display.display();                  //display data
}


//CALCULATE THE O2 CONCENTRATION
//**************************************************************************
void O2_CONCENTRATION() {
  adc0 = ads.readADC_SingleEnded(0); // Read ADC value from ADS1115

  //  Serial.println(factor);
  //  Serial.print("Analog pin 0: ");
  //Serial.println(adc0 * multiplier);

  //O2 = map(adc0, 80, 350, 20, 100);
  O2 = adc0 / factor;
  O2_FILTERED.Filter(O2);            //Calculate the filtered value
  O2_SMOOTH = O2_FILTERED.Current(); //calculate current filtered value

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
