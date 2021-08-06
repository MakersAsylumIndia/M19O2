#include "MegunoLink.h"
#include "CommandHandler.h"


float SliderValue = 0;

CommandHandler<> SerialCommandHandler;
InterfacePanel MyPanel;


void Cmd_SetSliderValue(CommandParameter &Parameters)
{
  Serial.print("SliderVal:");
  SliderValue=Parameters.NextParameterAsInteger();
  Serial.println(SliderValue);
  MyPanel.SetNumber("MySlider", 0); //Return the slider to zero position
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Snap Back To Zero Slider Example");
  Serial.println("-----------------------------");
  
  SerialCommandHandler.AddCommand(F("SliderVal"), Cmd_SetSliderValue);
  
}

void loop()
{
  SerialCommandHandler.Process();
}
