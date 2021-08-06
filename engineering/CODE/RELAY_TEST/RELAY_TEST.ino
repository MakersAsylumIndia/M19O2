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

#define SIEVE_A_PIN 5               //SIEVE A PIN
#define SIEVE_B_PIN 6               //SIEVE B PIN
#define COMP_RELAY 8                //COMP RELAY
#define BUTTON_PIN 4                //BUTTON PIN FOR CONTACTOR RELAY

unsigned long prevCycleTime = millis();
int           prevCycle = 4;
unsigned long waitTime = 0;

bool state = HIGH;           // the current state of the output pin
int reading;                 // the current reading from the input pin
bool previous = LOW;
unsigned long Time = 0;      // the last time the output pin was toggled
const int debounce = 200;


//Set delay timing here
//**************************************************************************
int CYCLE_TIME = 7000;
int FLUSH_TIME = 850;
int PRODUCTION_TIME = CYCLE_TIME - 2 * FLUSH_TIME;


//**************************************************************************
void setup() {
  Serial.begin(9600);       //serial communication for debugging
  Serial.println("Starting the machine");
  
  // SET PIN MODE FOR PINS IN PROGRAM
  //**************************************************************************
  pinMode(SIEVE_A_PIN, OUTPUT);
  pinMode(SIEVE_B_PIN, OUTPUT);
  
  pinMode(BUTTON_PIN, INPUT);
  pinMode(COMP_RELAY, OUTPUT);

  //Set initial states here
  //**************************************************************************
  //set SIEVE pins to LOW(turn off the relays)
  digitalWrite(SIEVE_A_PIN, HIGH);
  digitalWrite(SIEVE_B_PIN, HIGH);
}

void loop() {
  RUN_CYCLE();         //activate the sieves
  COMP_SWITCH();
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
  }
}

//CONTROL THE COMPRESSOR RELAY FROM COMPRESSOR SWITCH
//**************************************************************************
void COMP_SWITCH() {
  reading = digitalRead(BUTTON_PIN);
  Serial.println(reading);
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
