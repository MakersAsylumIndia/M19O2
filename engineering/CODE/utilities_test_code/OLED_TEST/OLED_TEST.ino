#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1305.h>

#define OLED_RESET 9
// I2C
Adafruit_SSD1305 display(128, 32, &Wire, OLED_RESET);


void setup()   {
  Serial.begin(9600);
  while (! Serial) delay(100);
  Serial.println("SSD1305 OLED test");

  if ( ! display.begin(0x3C) ) {
    Serial.println("Unable to initialize OLED");
    while (1) yield();
  }
  display.display();        // show splashscreen
  delay(1000);
  display.clearDisplay();   // clears the screen and buffer

  // init done
  display.display(); // show splashscreen
  delay(1000);
  display.clearDisplay();   // clears the screen and buffer

  delay(1000);
}



void loop() {
  testdrawchar();
  display.display();
  delay(1000);
  display.clearDisplay();
}



void testdrawchar(void) {
  display.setTextSize(1);
  display.setTextWrap(false);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  for (uint8_t i = 0; i < 168; i++) {
    if (i == '\n') continue;
    display.write(i);
    if ((i > 0) && (i % 21 == 0))
      display.println();
  }
  display.display();
}
