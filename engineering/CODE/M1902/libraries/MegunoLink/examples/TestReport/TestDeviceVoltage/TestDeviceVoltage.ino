/************************************************************************************************
Example Description
In this example we use MegunoLink's Device Test Panel to implement a test that checks the device
voltage and reports pass if its within an acceptable range or fail if not.
 
More Information
*  https://www.megunolink.com/documentation/program-device/test-monitor-panel/

This Example Requires:
*  The MegunoLink arduino library https://www.megunolink.com/documentation/arduino-integration/

MegunoLink Interface
You can download a pre-made interface from here:
https://github.com/Megunolink/MLP/raw/master/examples/TestReport/TestDeviceVoltage/TestDeviceVoltage.mlpz
 
You can find out more about MegunoLink and download a free trial from here
https://www.megunolink.com/
https://www.megunolink.com/download/
************************************************************************************************/

#include "MegunoLink.h"
#include "CommandHandler.h"

 
CommandHandler<> Commands;

// -----------------------------------------------------------------------------
// Self Test Implementation
void TestVcc()
{
  long BatteryVoltage = MeasureVcc();
  const long MinAcceptableBatteryVoltage = 4900; // mV
  const long MaxAcceptableBatteryVoltage = 5100; // mV
  bool bPass = MinAcceptableBatteryVoltage <= BatteryVoltage && BatteryVoltage <= MaxAcceptableBatteryVoltage;
 
  TestReport Test;

  Test.ReportResult("Power Supply", bPass, BatteryVoltage);
}

// -----------------------------------------------------------------------------
// Serial command handlers
void Cmd_Test(CommandParameter &p)
{
  TestVcc();
}

// -----------------------------------------------------------------------------
// Core Arduino implementation
void setup() 
{
  Serial.begin(9600);
  Serial.println(F("Test Panel Demo"));
  Serial.println(F("==============="));

  Commands.AddCommand(F("Test"), Cmd_Test);
}

void loop() 
{
  // Receive and dispatch serial commands.
  Commands.Process(); 
}

// -----------------------------------------------------------------------------
// Measures Vcc using the internal 1.1 V reference. See
// https://provideyourown.com/2012/secret-arduino-voltmeter-measure-battery-voltage/
long MeasureVcc() 
{
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
  ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  ADMUX = _BV(MUX3) | _BV(MUX2);
#elif defined(__AVR__)
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#else
#error Unsupported board
#endif  

  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA, ADSC)); // measuring

  uint8_t low = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both

  long result = (high << 8) | low;

  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}

