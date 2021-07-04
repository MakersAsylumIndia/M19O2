#include"oxikit.h"
#include<Arduino.h>

/*
Concentrator::Concentrator( const int& A_valve, 
                            const int& B_valve, 
                            const int& P_valve, 
                            const int& fan,
                            const int& relay_test_delay, 
                            const int& startup_purge_delay, 
                            const int& production_delay, 
                            const int& flush_delay, 
                            const int& precharge_delay) {

  Sieve_A_Valve = A_valve; 
  Sieve_B_Valve = B_valve; 
  PreCharge_Valve = P_valve; 
  Fan = fan;
  Serial.println("Relay pins set!");

  Serial.println("conctr object created ...");
  vTaskDelay(10/portTICK_PERIOD_MS);
  }
*/

void Concentrator::set_pinmode_for_program(void)
{
  pinMode(Sieve_A_Valve, OUTPUT);
  pinMode(Sieve_B_Valve, OUTPUT);
  pinMode(PreCharge_Valve, OUTPUT);
  pinMode(Fan, OUTPUT);
  Serial.println("Pinmode set successfully");
  vTaskDelay(10 / portTICK_PERIOD_MS);
}

/*
void Concentrator::set_relay_pins(const int& A_valve, 
                                  const int& B_valve, 
                                  const int& P_valve, 
                                  const int& fan){
  Sieve_A_Valve = A_valve; 
  Sieve_B_Valve = B_valve; 
  PreCharge_Valve = P_valve; 
  Fan = fan;
}

void Concentrator::set_delay_timing (const int& relay_test_delay, 
                                    const int& startup_purge_delay, 
                                    const int& production_delay, 
                                    const int& flush_delay, 
                                    const int& precharge_delay){
  Relay_Test_Delay = relay_test_delay;
  Startup_Purge_Delay = startup_purge_delay;
  Production_Delay = production_delay;
  Flush_Delay = flush_delay;
  PreCharge_Delay = precharge_delay;
}
*/

void Concentrator::valve_relay_test_sequence(void){
  Serial.println("Relay Test Sequence");
  digitalWrite(Sieve_A_Valve, HIGH); //Turn on relay
  vTaskDelay(Relay_Test_Delay/portTICK_PERIOD_MS);
  digitalWrite(Sieve_B_Valve, HIGH); //Turn on relay
  vTaskDelay(Relay_Test_Delay/portTICK_PERIOD_MS);
  digitalWrite(PreCharge_Valve, HIGH); //Turn on relay
  vTaskDelay(Relay_Test_Delay/portTICK_PERIOD_MS);
  Serial.println("Valve Relay Test Sequence Complete");
  vTaskDelay(Relay_Test_Delay/portTICK_PERIOD_MS);
}

void Concentrator::startup_purge(void){
  Serial.println("Startup Purge");
  digitalWrite(Sieve_A_Valve, HIGH); //Turn on relay
  digitalWrite(Sieve_B_Valve, HIGH); //Turn on relay
  digitalWrite(PreCharge_Valve, HIGH); //Turn on relay
  vTaskDelay(Startup_Purge_Delay/portTICK_PERIOD_MS);
}

void Concentrator::fan_control(void){
  Serial.println("Fan Control...");
  vTaskDelay(Relay_Test_Delay/portTICK_PERIOD_MS);
  digitalWrite(Fan, HIGH);
  Serial.println("Fan Switched On");
}

void Concentrator::cycle_1(void){
  Serial.println("cycle_1");
  Serial.println("Sieve A Charge / Sieve B Purge");
  digitalWrite(Sieve_A_Valve, HIGH);
  digitalWrite(Sieve_B_Valve, LOW);
  digitalWrite(PreCharge_Valve, LOW);
  vTaskDelay(Production_Delay/portTICK_PERIOD_MS);
}

void Concentrator::cycle_2(void){
  Serial.println("cycle_2");
  Serial.println("Sieve A Charge / Sieve B Purge / Flush/PreCharge");
  digitalWrite(Sieve_A_Valve, HIGH); 
  digitalWrite(Sieve_B_Valve, LOW);
  digitalWrite(PreCharge_Valve, HIGH);
  vTaskDelay(Flush_Delay/portTICK_PERIOD_MS) ;
}

void Concentrator::cycle_3(void){
  Serial.println("cycle_3");
  Serial.println("Sieve A Charge / Sieve B Charge / Flush/PreCharge");
  digitalWrite(Sieve_A_Valve, HIGH);
  digitalWrite(Sieve_B_Valve, HIGH);
  digitalWrite(PreCharge_Valve, HIGH);
  vTaskDelay(PreCharge_Delay/portTICK_PERIOD_MS);
}

void Concentrator::cycle_4(void){
  Serial.println("cycle_4");
  Serial.println("Sieve A Purge / Sieve B Charge");
  digitalWrite(Sieve_A_Valve, LOW);
  digitalWrite(Sieve_B_Valve, HIGH);
  digitalWrite(PreCharge_Valve, LOW);
  vTaskDelay(Production_Delay/portTICK_PERIOD_MS);
}

void Concentrator::cycle_5(void){
  Serial.println("cycle_5");
  Serial.println("Sieve A Purge / Sieve B Charge / Flush/PreCharge");
  digitalWrite(Sieve_A_Valve, LOW);
  digitalWrite(Sieve_B_Valve, HIGH);
  digitalWrite(PreCharge_Valve, HIGH);
  vTaskDelay(Flush_Delay/portTICK_PERIOD_MS);
}

void Concentrator::cycle_6(void){
  Serial.println("cycle_6");
  Serial.println("Sieve A Charge / Sieve B Charge / Flush/PreCharge");
  digitalWrite(Sieve_A_Valve, HIGH);
  digitalWrite(Sieve_B_Valve, HIGH);
  digitalWrite(PreCharge_Valve, HIGH);
  vTaskDelay(PreCharge_Delay/portTICK_PERIOD_MS) ;
}

void Concentrator::run(void){
  cycle_1();
  cycle_2();
  cycle_3();
  cycle_4();
  cycle_5();
  cycle_6();
}

void Concentrator::ConctrRun_Task(void *parameters){
  Serial.println("Oxikit Brain by ME Team!");
  set_pinmode_for_program();
  valve_relay_test_sequence();
  startup_purge();
  fan_control();
  for(;;){
    run();
  }
}


void Concentrator::begin(){
  xTaskCreate(
     ConctrRun_Task, //function that should be called
    "Concentrator Run",
    2000,
    NULL,
    1,
    NULL //task handler
    );

}


