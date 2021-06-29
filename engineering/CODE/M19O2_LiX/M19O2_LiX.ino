/*
  M19O2_LiX

  Control code for Lithium based M19O2 oxygen cnecentrator.
  WORK IN PROGRESS
  Uses a single 5/2 solenoid valve (or two 3/2 valves) - one each at the input of the LiX sieves.
  There is no equalization valve.
  Only two orifices - one each at the output of each sieve.
  Input to surge tank goes via a non-return valve (NRV)

  This code is in the public domain. LICENSE = MIT

  Hardware and code by Anool J M (@anool) and Narender Sharma of Maker's Asylum

  https://github.com/MakersAsylumIndia/M19O2/tree/main/engineering/software/M19O2_LiX
  

*/


//****************************************************************
#include <EEPROM.h>

#include "uptime.h"
#include "uptime_formatter.h"

void runCycle();
void updateTime();

int currentTimeSeconds;
int noOfhours;
int noOfDays;
bool DaysWritten;

// RELAY PIN ASSIGNMENT
//**************************************************************************
#define SIEVE_A_PIN 5
#define SIEVE_B_PIN 6

unsigned long prevCycleTime = millis();
int prevCycle = 4;
unsigned long waitTime = 0;

int CYCLE_TIME = 7000;
int FLUSH_TIME = 850;
int PRODUCTION_TIME = CYCLE_TIME-2* FLUSH_TIME;

//**************************************************************************
void setup()
{
  Serial.begin(9600);
  EEPROM.begin();
  noOfDays = EEPROM.read(5);
  noOfhours = EEPROM.read(1);
  // STARTUP
  Serial.begin(9600);


  // SET PIN MODE FOR PINS IN PROGRAM
  //**************************************************************************
  pinMode(SIEVE_A_PIN, OUTPUT);
  pinMode(SIEVE_B_PIN, OUTPUT);

  digitalWrite(SIEVE_A_PIN, HIGH);
  digitalWrite(SIEVE_B_PIN, HIGH);

  //  SET DELAY TIMING HERE
  //**************************************************************************



}

void loop()
{
  runCycle();
  updateTime();
}




void runCycle() {
  //CYCLE 1
  //*****************************************************
  if ((millis() - prevCycleTime >= waitTime) && (prevCycle == 4)) {
    digitalWrite(SIEVE_A_PIN, LOW);
    digitalWrite(SIEVE_B_PIN, LOW);
    Serial.println("SEIVE A on, SEIVER B, on");
    prevCycleTime = millis();
    prevCycle = 1;
    waitTime = FLUSH_TIME;
    Serial.println(FLUSH_TIME);
  }

  //CYCLE2
  //****************************************************
  if ((millis() - prevCycleTime >= waitTime) && (prevCycle == 1)) {
    digitalWrite(SIEVE_B_PIN, HIGH);
    Serial.println("SEIVE A on, SEIVER B, off");
    prevCycleTime = millis();
    prevCycle = 2;
    waitTime = PRODUCTION_TIME;
    Serial.println(PRODUCTION_TIME);
  }

  //CYCLE 3
  //****************************************************
  if ((millis() - prevCycleTime >= waitTime) && (prevCycle == 2)) {
    digitalWrite(SIEVE_B_PIN, LOW);
    Serial.println("SEIVE A on, SEIVER B, on");
    prevCycleTime = millis();
    prevCycle = 3;
    waitTime = FLUSH_TIME;
  Serial.println(FLUSH_TIME);
  }

  //CYCLE 4
  //***************************************************
  if ((millis() - prevCycleTime >= waitTime) && (prevCycle == 3)) {
    digitalWrite(SIEVE_A_PIN, HIGH);
    Serial.println("SEIVE A off, SEIVER B, on");
    prevCycleTime = millis();
    prevCycle = 4;
    waitTime = PRODUCTION_TIME;
    Serial.println(PRODUCTION_TIME);
  }


}
void updateTime() {
  DaysWritten = false;
  uptime::calculateUptime();

  if (uptime::getHours() != currentTimeSeconds) {
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
