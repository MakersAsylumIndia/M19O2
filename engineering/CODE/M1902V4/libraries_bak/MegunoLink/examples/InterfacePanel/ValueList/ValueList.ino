/* **********************************************************************************************
*  Example program to select list box values on a MegunoLink Pro User Interface Panel. 
*  
*  This program periodically sends out commands to select items in two ValueList controls. Values 
*  in the first control (named lbUser) are selected by index. Values in the second ValueList 
*  control are selected by value. 
*  ********************************************************************************************** */

#include "MegunoLink.h" // Helpful functions for communicating with MegunoLink Pro. 

// Millis value when the data was last sent. 
long LastSent;

// Currently selected 'user' index value
int UserIndex; 

// User ages we select
int UserAge[] = { 3, 5, 22 };
const int UserAgeCount = sizeof(UserAge)/sizeof(UserAge[0]);

// The user age we are going to select next
int UserAgeIndex;

// The interface panel we are sending data to
InterfacePanel Panel("Test");

void setup()
{
  Serial.begin(9600);

  LastSent = millis();
  UserIndex = 0;
  UserAgeIndex = 0;
}

void loop()
{
  if ((millis() - LastSent) > 1500)
  {
    LastSent = millis();

    // Select the next item by index. 
    Panel.SetListIndex("lbUser", UserIndex);
    UserIndex = (UserIndex + 1) % 3;

    // Select an item by its assigned value. 
    Panel.SetListValue("lbUserAge", UserAge[UserAgeIndex]);
    UserAgeIndex = (UserAgeIndex + 1) % UserAgeCount;
  }
}
