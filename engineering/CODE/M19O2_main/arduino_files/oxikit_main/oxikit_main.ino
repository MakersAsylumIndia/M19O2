#include <Arduino.h>
#include <oxikit_nano.h>
#include<Arduino_FreeRTOS.h>

using  Cr = Concentrator;

const int Cr::Sieve_A_Valve = 5; 
const int Cr::Sieve_B_Valve = 6; 
const int Cr::PreCharge_Valve = 7; 
const int Cr::Fan = 8; 

const int Cr::Relay_Test_Delay = 0;
const int Cr::Startup_Purge_Delay = 1000;
const int Cr::Production_Delay = 4000;
const int Cr::Flush_Delay = 450;
const int Cr::PreCharge_Delay = 700;

void ConctrRun_Task(void *pvParameters){
  (void) pvParameters;
  Serial.println("Oxikit Brain by ME Team!");
  Cr::set_pinmode_for_program();
  Cr::valve_relay_test_sequence();
  Cr::startup_purge();
  Cr::fan_control();
  for(;;){
    Cr::run();
  }
}

TaskHandle_t ConctrRun_Task_handle ;

void setup() {
  Serial.begin(9600);
  Cr::begin();
}

void loop() {
  }
