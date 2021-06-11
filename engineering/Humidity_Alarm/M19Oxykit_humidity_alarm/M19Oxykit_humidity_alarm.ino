/*
  M19O2_Humidity_Alarm

  Reads a DHT-22 sensor attached in-line between the output of Dessicant Sieve and input of high pressure regulator of the M19O2 Oxygen Concentrator.
  This tells us how much moisture is entering the Zeolite Dessicant Sieves.
  >50%rH = Red Blinking LED + Buzzer
  <50%rH, >30%rH = Red Blinking LED
  <30%rH, >15%rH = Blinking Green LED
  <15%rH = Steady Green LED

  The DHT22 can only measure under ambient pressure (1 ATM), so we tap off the high pressure line and feed air through restrictors (orifices) in to an air-sealed chamber containing the DHT-22.
  The output from the air sealed chamber feeds back in to the main line, again through a similar restrictor orifice.

  In a later iteration, a flushing solenoid valve can be connected which will purge incomming air back to atmosphere until humidity level falls below a set percentage, thus preventing humid air from reaching the sieves.

  This code is in the public domain.

  Hardware and code by Anool J M (@anool) and Narender Sharma of Maker's Asylum

  https://github.com/MakersAsylumIndia/M19_OxiKit/tree/main/engineering/M19Oxykit_humidity_alarm

*/


//include DHT libaries
#include "DHT.h"

//define DHT parameters
#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

//create DHT object
DHT dht(DHTPIN, DHTTYPE);

//variable for humidity and temperature
float h;
float t;

//RGB led connected to Digital pins
int RED = 3;
int GREEN = 5;
int BLUE = 6;

//buzzer pin
int BUZZER = 4;

void setup() {
//serial communication
  Serial.begin(9600);

//define pins as input &output
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  pinMode(BUZZER, OUTPUT);

//Set LED pins to LOW(turn off the LED)
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, HIGH);

//begin dht communication
  dht.begin();
}

void loop() {
  getTemp();        //get the temperature and humidity
  ledStatus(h);     //set led colors and trigger alarm

}

void getTemp() {
  h = dht.readHumidity();
  t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

//  Serial.print(F("Humidity: "));
//  Serial.print(h);
//  Serial.print(F("%  Temperature: "));
//  Serial.print(t);
//  Serial.println(F("°C "));
//  Serial.println("");

}

void ledFunction(int h) {
  if (h <= 15) {                    //Green color if humidity is less than 15
    analogWrite(RED, 255);          //turn off red
    analogWrite(GREEN, 180);        //turn on Green
//  Serial.print("less than 15");
  }

  else if (h > 15 && h <= 30) {     //blink green led if humidity is from 15 to 30 percent
    analogWrite(RED, 255);          //turn off red
//  Serial.print("less than 30");
    analogWrite(GREEN, 180);        //turn on green
    delay(200);
    analogWrite(GREEN, 255);        //turn off green
    delay(200);
  }

  else if (h > 30 && h <= 50) {     //turn on red led if humidity is from 30 to 50 percent
//  Serial.print("less than 50");   //turn off green
    analogWrite(GREEN, 255);
    analogWrite(RED, 180);          //turn on red
  }

  else if (h > 50) {                //blink red led and trigger the alarm
//  Serial.print("more than 50");
    analogWrite(GREEN, 255);        //turn off green
    tone(BUZZER, 300);
    analogWrite(RED, 180);          //turn on red
    delay(200);
    noTone(BUZZER);
    analogWrite(RED, 255);          //turn off red
    delay(200);
  }

  else {
    analogWrite(RED, 255);
    analogWrite(GREEN, 255);
    analogWrite(BLUE, 255);
    noTone(BUZZER);
  }

}
