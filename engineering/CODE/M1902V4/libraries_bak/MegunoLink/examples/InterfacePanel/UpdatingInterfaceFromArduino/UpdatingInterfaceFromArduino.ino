#include <MegunoLink.h>

InterfacePanel MyPanel;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  // Demonstrate many of the options around updating the interface panel.
  MyPanel.SetText(F("Item"), F(""));
  MyPanel.SetText(F("DescriptionTextbox"), F(""));
  MyPanel.SetText(F("DescriptionTextbox"), F("Demo Starting"));
  delay(5000);
  MyPanel.SetText(F("DescriptionTextbox"), F(""));
  delay(500);
  MyPanel.SetText(F("DescriptionTextbox"), F("You can edit various controls from your arduino."));
  MyPanel.SetText(F("Item"), F("including buttons,"));

  //Buttons
  MyPanel.HideControl(F("MyButton"));
  delay(500);
  MyPanel.ShowControl(F("MyButton"));
  delay(500);
  MyPanel.HideControl(F("MyButton"));
  delay(500);
  MyPanel.ShowControl(F("MyButton"));
  delay(500);
  MyPanel.HideControl(F("MyButton"));
  delay(500);
  MyPanel.ShowControl(F("MyButton"));
  delay(500);
  MyPanel.DisableControl(F("MyButton"));
  delay(500);
  MyPanel.EnableControl(F("MyButton"));
  delay(500);
  MyPanel.DisableControl(F("MyButton"));
  delay(500);
  MyPanel.EnableControl(F("MyButton"));
  delay(500);
  MyPanel.DisableControl(F("MyButton"));
  delay(500);
  MyPanel.EnableControl(F("MyButton"));
  delay(500);
  MyPanel.SetForeColor(F("MyButton"), F("blue"));
  delay(500);
  MyPanel.SetForeColor(F("MyButton"), F("red"));
  delay(500);
  MyPanel.SetForeColor(F("MyButton"), F("green"));
  delay(500);
  MyPanel.SetForeColor(F("MyButton"), F("black"));
  delay(500);
  MyPanel.SetBackColor(F("MyButton"), F("blue"));
  delay(500);
  MyPanel.SetBackColor(F("MyButton"), F("red"));
  delay(500);
  MyPanel.SetBackColor(F("MyButton"), F("green"));
  delay(500);
  MyPanel.SetBackColor(F("MyButton"), F("white"));
  delay(500);


  //Textboxes
  MyPanel.SetText(F("Item"), F("including buttons, textboxes"));
  delay(500);
  MyPanel.SetText(F("MyTextbox"), F("you"));
  delay(500);
  MyPanel.SetText(F("MyTextbox"), F("can"));
  delay(500);
  MyPanel.SetText(F("MyTextbox"), F("edit"));
  delay(500);
  MyPanel.SetText(F("MyTextbox"), F("textboxes"));
  delay(500);
  MyPanel.SetText(F("MyTextbox"), F("from"));
  delay(500);
  MyPanel.SetText(F("MyTextbox"), F("your"));
  delay(500);
  MyPanel.SetText(F("MyTextbox"), F("arduino"));
  delay(500);


  //Gauges
  MyPanel.SetText(F("Item"), F("including buttons, textboxes, gauges"));
  delay(500);
  MyPanel.SetGaugeLabel(F("MyGauge"),0, F("you"));
  delay(500);
  MyPanel.SetGaugeLabel(F("MyGauge"),0, F("can"));
  delay(500);
  MyPanel.SetGaugeLabel(F("MyGauge"),0, F("edit"));
  delay(500);
  MyPanel.SetGaugeLabel(F("MyGauge"),0, F("gauge"));
  delay(500);
  MyPanel.SetGaugeLabel(F("MyGauge"),0, F("labels"));
  delay(500);
  MyPanel.SetGaugeLabel(F("MyGauge"),0, F("from"));
  delay(500);
  MyPanel.SetGaugeLabel(F("MyGauge"),0, F("your"));
  delay(500);
  MyPanel.SetGaugeLabel(F("MyGauge"),0, F("arduino"));
  delay(500);
  MyPanel.SetNumber(F("MyGauge"), 100);
  delay(500);
  MyPanel.SetNumber(F("MyGauge"), 200);
  delay(500);
  MyPanel.SetNumber(F("MyGauge"), 300);
  delay(500);
  MyPanel.SetNumber(F("MyGauge"), 400);
  delay(500);
  MyPanel.SetNumber(F("MyGauge"), 500);
  delay(500);
  

  //Checkboxes
  MyPanel.SetText(F("Item"), F("including buttons, textboxes, gauges, checkboxes"));
  delay(500);
  MyPanel.SetCheck(F("MyCheckbox"));
  delay(500);
  MyPanel.ClearCheck(F("MyCheckbox"));
  delay(500);
  MyPanel.SetCheck(F("MyCheckbox"));
  delay(500);
  MyPanel.ClearCheck(F("MyCheckbox"));
  delay(500);
  MyPanel.SetCheck(F("MyCheckbox"));
  delay(500);
  MyPanel.ClearCheck(F("MyCheckbox"));
  delay(500);
  MyPanel.SetCheck(F("MyCheckbox"));
  delay(500);
  MyPanel.ClearCheck(F("MyCheckbox"));
  delay(500);


  //Trackbars
  MyPanel.SetText(F("Item"), F("trackbars"));
  delay(500);
  MyPanel.SetNumber(F("MyTrackbar"), 10);
  delay(500);
  MyPanel.SetNumber(F("MyTrackbar"), 20);
  delay(500);
  MyPanel.SetNumber(F("MyTrackbar"), 30);
  delay(500);
  MyPanel.SetNumber(F("MyTrackbar"), 40);
  delay(500);
  MyPanel.SetNumber(F("MyTrackbar"), 50);
  delay(500);


  //Progress bars
  MyPanel.SetText(F("Item"), F("trackbars, progress bars"));
  delay(500);
  MyPanel.SetProgress(F("MyProgressBar"), 10);
  delay(500);
  MyPanel.SetProgress(F("MyProgressBar"), 20);
  delay(500);
  MyPanel.SetProgress(F("MyProgressBar"), 30);
  delay(500);
  MyPanel.SetProgress(F("MyProgressBar"), 40);
  delay(500);
  MyPanel.SetProgress(F("MyProgressBar"), 50);
  delay(500);


  //Pictures
  MyPanel.SetText(F("Item"), F("trackbars, progress bars, pictures"));
  delay(500);
  MyPanel.HideControl(F("PausePicture"));
  MyPanel.ShowControl(F("PlayPicture"));
  delay(500);
  MyPanel.ShowControl(F("PausePicture"));
  MyPanel.HideControl(F("PlayPicture"));
  delay(500);
  MyPanel.HideControl(F("PausePicture"));
  MyPanel.ShowControl(F("PlayPicture"));
  delay(500);
  MyPanel.ShowControl(F("PausePicture"));
  MyPanel.HideControl(F("PlayPicture"));
  delay(500);
  MyPanel.HideControl(F("PausePicture"));
  MyPanel.ShowControl(F("PlayPicture"));
  delay(500);
  MyPanel.ShowControl(F("PausePicture"));
  MyPanel.HideControl(F("PlayPicture"));
  delay(500);


  MyPanel.SetText(F("DescriptionTextbox"), F("You can also have tab pages, value lists, radio buttons,"));
  MyPanel.SetText(F("Item"), F("group boxes, and buttons to launch files."));
  delay(10000);
}
