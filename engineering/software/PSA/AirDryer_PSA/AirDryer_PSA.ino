//****************************************************************
void runCycle();


// RELAY PIN ASSIGNMENT
//**************************************************************************
#define SIEVE_A_PIN 5
#define SIEVE_B_PIN 6
#define SIEVE_C_PIN 7
#define SIEVE_D_PIN 8

unsigned long prevCycleTime = millis();
int prevCycle = 4;
unsigned long waitTime = 0;

int CYCLE_TIME = 60000;
int FLUSH_TIME = 5000;
int PRODUCTION_TIME = CYCLE_TIME - 2 * FLUSH_TIME;

//**************************************************************************
void setup()
{
  Serial.begin(9600);

  // SET PIN MODE FOR PINS IN PROGRAM
  //**************************************************************************
  pinMode(SIEVE_A_PIN, OUTPUT);
  pinMode(SIEVE_B_PIN, OUTPUT);
  pinMode(SIEVE_C_PIN, OUTPUT);
  pinMode(SIEVE_D_PIN, OUTPUT);

  digitalWrite(SIEVE_A_PIN, HIGH);
  digitalWrite(SIEVE_B_PIN, HIGH);
  digitalWrite(SIEVE_C_PIN, HIGH);
  digitalWrite(SIEVE_D_PIN, HIGH);
  delay(2000);
}

void loop()
{
  runCycle();
}

void runCycle() {
  //CYCLE 1
  //*****************************************************
  if ((millis() - prevCycleTime >= waitTime) && (prevCycle == 4)) {
    digitalWrite(SIEVE_A_PIN, LOW);
    digitalWrite(SIEVE_B_PIN, LOW);
    digitalWrite(SIEVE_C_PIN, HIGH);
    digitalWrite(SIEVE_D_PIN, HIGH);
    Serial.println("Sieve A: ON, Sieve B: ON, Sieve C: OFF, Sieve D: OFF");

    prevCycleTime = millis();
    prevCycle = 1;
    waitTime = PRODUCTION_TIME;
  }

  //CYCLE2
  //****************************************************
  if ((millis() - prevCycleTime >= waitTime) && (prevCycle == 1)) {
    digitalWrite(SIEVE_A_PIN, LOW);
    digitalWrite(SIEVE_B_PIN, LOW);
    digitalWrite(SIEVE_C_PIN, LOW);
    digitalWrite(SIEVE_D_PIN, LOW);
    Serial.println("Sieve A: ON, Sieve B: ON, Sieve C: ON, Sieve D: ON");
    
    prevCycleTime = millis();
    prevCycle = 2;
    waitTime = FLUSH_TIME;
  }

  //CYCLE 3
  //****************************************************
  if ((millis() - prevCycleTime >= waitTime) && (prevCycle == 2)) {
    digitalWrite(SIEVE_A_PIN, HIGH);
    digitalWrite(SIEVE_B_PIN, HIGH);
    digitalWrite(SIEVE_C_PIN, LOW);
    digitalWrite(SIEVE_D_PIN, LOW);
    Serial.println("Sieve A: OFF, Sieve B: OFF, Sieve C: ON, Sieve D: ON");
    
    prevCycleTime = millis();
    prevCycle = 3;
    waitTime = PRODUCTION_TIME;
  }

  //CYCLE 4
  //***************************************************
  if ((millis() - prevCycleTime >= waitTime) && (prevCycle == 3)) {
    digitalWrite(SIEVE_A_PIN, LOW);
    digitalWrite(SIEVE_B_PIN, LOW);
    digitalWrite(SIEVE_C_PIN, LOW);
    digitalWrite(SIEVE_D_PIN, LOW);
    Serial.println("Sieve A: ON, Sieve B: ON, Sieve C: ON, Sieve D: ON");
    
    prevCycleTime = millis();
    prevCycle = 4;
    waitTime = FLUSH_TIME;
  }

}
