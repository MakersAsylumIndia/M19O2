/*********************************************************************
This is a library for our Monochrome OLEDs based on SSD1305 drivers

  Pick one up today in the adafruit shop!
  ------> https://www.adafruit.com/products/2675

These displays use I2C or SPI to communicate

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.
BSD license, check license.txt for more information
All text above, and the splash screen below must be included in any
redistribution
*********************************************************************/

#include "Adafruit_SSD1305.h"
#include "splash.h"

// CONSTRUCTORS, DESTRUCTOR ------------------------------------------------

/*!
    @brief  Constructor for I2C-interfaced SSD1305 displays.
    @param  w
            Display width in pixels
    @param  h
            Display height in pixels
    @param  twi
            Pointer to an existing TwoWire instance (e.g. &Wire, the
            microcontroller's primary I2C bus).
    @param  rst_pin
            Reset pin (using Arduino pin numbering), or -1 if not used
            (some displays might be wired to share the microcontroller's
            reset pin).
    @param  clkDuring
            Speed (in Hz) for Wire transmissions in SSD1305 library calls.
            Defaults to 400000 (400 KHz), a known 'safe' value for most
            microcontrollers, and meets the SSD1305 datasheet spec.
            Some systems can operate I2C faster (800 KHz for ESP32, 1 MHz
            for many other 32-bit MCUs), and some (perhaps not all)
            SSD1305's can work with this -- so it's optionally be specified
            here and is not a default behavior. (Ignored if using pre-1.5.7
            Arduino software, which operates I2C at a fixed 100 KHz.)
    @param  clkAfter
            Speed (in Hz) for Wire transmissions following SSD1305 library
            calls. Defaults to 100000 (100 KHz), the default Arduino Wire
            speed. This is done rather than leaving it at the 'during' speed
            because other devices on the I2C bus might not be compatible
            with the faster rate. (Ignored if using pre-1.5.7 Arduino
            software, which operates I2C at a fixed 100 KHz.)
    @note   Call the object's begin() function before use -- buffer
            allocation is performed there!
*/
Adafruit_SSD1305::Adafruit_SSD1305(uint16_t w, uint16_t h, TwoWire *twi,
                                   int8_t rst_pin, uint32_t clkDuring,
                                   uint32_t clkAfter)
    : Adafruit_GrayOLED(1, w, h, twi, rst_pin, clkDuring, clkAfter) {}

/*!
    @brief  Constructor for SPI SSD1305 displays, using software (bitbang)
            SPI.
    @param  w
            Display width in pixels
    @param  h
            Display height in pixels
    @param  mosi_pin
            MOSI (master out, slave in) pin (using Arduino pin numbering).
            This transfers serial data from microcontroller to display.
    @param  sclk_pin
            SCLK (serial clock) pin (using Arduino pin numbering).
            This clocks each bit from MOSI.
    @param  dc_pin
            Data/command pin (using Arduino pin numbering), selects whether
            display is receiving commands (low) or data (high).
    @param  rst_pin
            Reset pin (using Arduino pin numbering), or -1 if not used
            (some displays might be wired to share the microcontroller's
            reset pin).
    @param  cs_pin
            Chip-select pin (using Arduino pin numbering) for sharing the
            bus with other devices. Active low.
    @note   Call the object's begin() function before use -- buffer
            allocation is performed there!
*/
Adafruit_SSD1305::Adafruit_SSD1305(uint16_t w, uint16_t h, int8_t mosi_pin,
                                   int8_t sclk_pin, int8_t dc_pin,
                                   int8_t rst_pin, int8_t cs_pin)
    : Adafruit_GrayOLED(1, w, h, mosi_pin, sclk_pin, dc_pin, rst_pin, cs_pin) {}

/*!
    @brief  Constructor for SPI SSD1305 displays, using native hardware SPI.
    @param  w
            Display width in pixels
    @param  h
            Display height in pixels
    @param  spi
            Pointer to an existing SPIClass instance (e.g. &SPI, the
            microcontroller's primary SPI bus).
    @param  dc_pin
            Data/command pin (using Arduino pin numbering), selects whether
            display is receiving commands (low) or data (high).
    @param  rst_pin
            Reset pin (using Arduino pin numbering), or -1 if not used
            (some displays might be wired to share the microcontroller's
            reset pin).
    @param  cs_pin
            Chip-select pin (using Arduino pin numbering) for sharing the
            bus with other devices. Active low.
    @param  bitrate
            SPI clock rate for transfers to this display. Default if
            unspecified is 8000000UL (8 MHz).
    @note   Call the object's begin() function before use -- buffer
            allocation is performed there!
*/
Adafruit_SSD1305::Adafruit_SSD1305(uint16_t w, uint16_t h, SPIClass *spi,
                                   int8_t dc_pin, int8_t rst_pin, int8_t cs_pin,
                                   uint32_t bitrate)
    : Adafruit_GrayOLED(1, w, h, spi, dc_pin, rst_pin, cs_pin, bitrate) {}

/*!
    @brief  Destructor for Adafruit_SSD1305 object.
*/
Adafruit_SSD1305::~Adafruit_SSD1305(void) {}

// ALLOCATE & INIT DISPLAY -------------------------------------------------

/*!
    @brief  Allocate RAM for image buffer, initialize peripherals and pins.
    @param  addr
            I2C address of corresponding SSD1305 display.
            SPI displays (hardware or software) do not use addresses, but
            this argument is still required (pass 0 or any value really,
            it will simply be ignored). Default if unspecified is 0.
    @param  reset
            If true, and if the reset pin passed to the constructor is
            valid, a hard reset will be performed before initializing the
            display. If using multiple SSD1305 displays on the same bus, and
            if they all share the same reset pin, you should only pass true
            on the first display being initialized, false on all others,
            else the already-initialized displays would be reset. Default if
            unspecified is true.
    @return true on successful allocation/init, false otherwise.
            Well-behaved code should check the return value before
            proceeding.
    @note   MUST call this function before any drawing or updates!
*/
bool Adafruit_SSD1305::begin(uint8_t addr, bool reset) {

  if (!Adafruit_GrayOLED::_init(addr, reset)) {
    return false;
  }

  drawBitmap((WIDTH - splash2_width) / 2, (HEIGHT - splash2_height) / 2,
             splash2_data, splash2_width, splash2_height, 1);

  // Init sequence, make sure its under 32 bytes, or split into multiples!
  static const uint8_t init_128x32[] = {
      // Init sequence for 128x32 OLED module
      SSD1305_DISPLAYOFF,          // 0xAE
      SSD1305_SETLOWCOLUMN | 0x0,  // low col = 0
      SSD1305_SETHIGHCOLUMN | 0x0, // hi col = 0
      SSD1305_SETSTARTLINE | 0x0,  // line #0
      0x2E,                        // ??
      SSD1305_SETCONTRAST,
      0x32, // 0x81, 0x32
      SSD1305_SETBRIGHTNESS,
      0x10, // 0x82, 0x10
      SSD1305_SEGREMAP | 0x01,
      SSD1305_NORMALDISPLAY, // 0xA6
      SSD1305_SETMULTIPLEX,
      0x3F, // 0xA8, 0x3F (1/64)
      SSD1305_MASTERCONFIG,
      0x8E, // external vcc supply
      SSD1305_COMSCANDEC,
      SSD1305_SETDISPLAYOFFSET,
      0x40, // 0xD3, 0x40
      SSD1305_SETDISPLAYCLOCKDIV,
      0xF0, // 0xD5, 0xF0
      SSD1305_SETAREACOLOR,
      0x05,
      SSD1305_SETPRECHARGE,
      0xF1, // 0xd9, 0xF1
      SSD1305_SETCOMPINS,
      0x12, // 0xDA, 0x12
      SSD1305_SETLUT,
      0x3F,
      0x3F,
      0x3F,
      0x3F};
  static const uint8_t init_128x64[] = {
      // Init sequence for 128x32 OLED module
      SSD1305_DISPLAYOFF,          // 0xAE
      SSD1305_SETLOWCOLUMN | 0x4,  // low col = 0
      SSD1305_SETHIGHCOLUMN | 0x4, // hi col = 0
      SSD1305_SETSTARTLINE | 0x0,  // line #0
      0x2E,                        //??
      SSD1305_SETCONTRAST,
      0x32, // 0x81, 0x32
      SSD1305_SETBRIGHTNESS,
      0x80, // 0x82, 0x80
      SSD1305_SEGREMAP | 0x01,
      SSD1305_NORMALDISPLAY, // 0xA6
      SSD1305_SETMULTIPLEX,
      0x3F, // 0xA8, 0x3F (1/64)
      SSD1305_MASTERCONFIG,
      0x8E, /* external vcc supply */
      SSD1305_COMSCANDEC,
      SSD1305_SETDISPLAYOFFSET,
      0x40, // 0xD3, 0x40
      SSD1305_SETDISPLAYCLOCKDIV,
      0xF0, // 0xD5, 0xF0
      SSD1305_SETAREACOLOR,
      0x05,
      SSD1305_SETPRECHARGE,
      0xF1, // 0xd9, 0xF1
      SSD1305_SETCOMPINS,
      0x12, // 0xDA, 0x12
      SSD1305_SETLUT,
      0x3F,
      0x3F,
      0x3F,
      0x3F};

  if (HEIGHT == 32) {
    page_offset = 4;
    column_offset = 4;
    if (!oled_commandList(init_128x32, sizeof(init_128x32))) {
      return false;
    }
  } else {
    // 128x64 high
    page_offset = 0;
    if (!oled_commandList(init_128x64, sizeof(init_128x64))) {
      return false;
    }
  }
  delay(100);                      // 100ms delay recommended
  oled_command(SSD1305_DISPLAYON); // 0xaf
  setContrast(0x2F);

  return true; // Success
}

/*!
    @brief  Do the actual writing of the internal frame buffer to display RAM
*/
void Adafruit_SSD1305::display(void) {
  // ESP8266 needs a periodic yield() call to avoid watchdog reset.
  // With the limited size of SSD1305 displays, and the fast bitrate
  // being used (1 MHz or more), I think one yield() immediately before
  // a screen write and one immediately after should cover it.  But if
  // not, if this becomes a problem, yields() might be added in the
  // 32-byte transfer condition below.
  yield();

  // uint16_t count = WIDTH * ((HEIGHT + 7) / 8);
  uint8_t *ptr = buffer;
  uint8_t dc_byte = 0x40;
  // uint8_t pages = ((HEIGHT + 7) / 8);

  uint8_t bytes_per_page = WIDTH;
  uint8_t maxbuff = 128;

  /*
  Serial.print("Window: (");
  Serial.print(window_x1);
  Serial.print(", ");
  Serial.print(window_y1);
  Serial.print(" -> (");
  Serial.print(window_x2);
  Serial.print(", ");
  Serial.print(window_y2);
  Serial.println(")");
  */

  uint8_t first_page = window_y1 / 8;
  uint8_t last_page = (window_y2 + 7) / 8;
  uint8_t page_start = min((int16_t)bytes_per_page, window_x1);
  uint8_t page_end = max((int16_t)0, window_x2);
  /*
  Serial.print("Pages: ");
  Serial.print(first_page);
  Serial.print(" -> ");
  Serial.println(last_page);
  pages = min(pages, last_page);

  Serial.print("Page addr: ");
  Serial.print(page_start);
  Serial.print(" -> ");
  Serial.println(page_end);
  */

  if (i2c_dev) { // I2C
    // Set high speed clk
    i2c_dev->setSpeed(i2c_preclk);
    maxbuff = i2c_dev->maxBufferSize() - 1;
  }

  for (uint8_t p = first_page; p < last_page; p++) {
    uint8_t bytes_remaining = bytes_per_page;
    ptr = buffer + (uint16_t)p * (uint16_t)bytes_per_page;
    // fast forward to dirty rectangle beginning
    ptr += page_start;
    bytes_remaining -= page_start;
    // cut off end of dirty rectangle
    bytes_remaining -= (WIDTH - 1) - page_end;

    uint8_t cmd[] = {uint8_t(SSD1305_SETPAGESTART + p + page_offset),
                     uint8_t(0x10 + ((page_start + column_offset) >> 4)),
                     uint8_t((page_start + column_offset) & 0xF)};
    oled_commandList(cmd, sizeof(cmd));

    while (bytes_remaining) {
      uint8_t to_write = min(bytes_remaining, maxbuff);
      if (i2c_dev) {
        i2c_dev->write(ptr, to_write, true, &dc_byte, 1);
      } else {
        digitalWrite(dcPin, HIGH);
        spi_dev->write(ptr, to_write);
      }
      ptr += to_write;
      bytes_remaining -= to_write;
      yield();
    }
  }

  if (i2c_dev) { // I2C
    // Set low speed clk
    i2c_dev->setSpeed(i2c_postclk);
  }

  // reset dirty window
  window_x1 = 1024;
  window_y1 = 1024;
  window_x2 = -1;
  window_y2 = -1;
}

/*!
    @brief  Put the display driver into a low power mode instead of just turning
   off all pixels
*/
void Adafruit_SSD1305::sleep(void) { oled_command(SSD1305_DISPLAYOFF); }

/*!
    @brief  Wake the display driver from the low power mode
*/
void Adafruit_SSD1305::wake(void) { oled_command(SSD1305_DISPLAYON); }
