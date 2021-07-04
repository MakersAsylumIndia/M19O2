#include <Arduino.h>
#include <oxikit.h>

using  Cr = Concentrator;

const int Cr::Sieve_A_Valve = 32; 
const int Cr::Sieve_B_Valve = 33; 
const int Cr::PreCharge_Valve = 25; 
const int Cr::Fan = 26; 

const int Cr::Relay_Test_Delay = 0;
const int Cr::Startup_Purge_Delay = 1000;
const int Cr::Production_Delay = 4000;
const int Cr::Flush_Delay = 450;
const int Cr::PreCharge_Delay = 700;

TaskHandle_t ConctrRun_Task_handle ;

void setup() {
  Serial.begin(9600);
  Cr::begin();
}

void loop() {
  }