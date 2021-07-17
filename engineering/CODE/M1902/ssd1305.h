/**
  ******************************************************************************
  * @file    SSD1305.c 
  * @author  Waveshare Team
  * @version 
  * @date    Jule-30-2019
  * @brief   This file includes the OLED driver for SSD1305 display moudle
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, WAVESHARE SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  ******************************************************************************
  */
  
#ifndef _SSD1305_H_
#define _SSD1305_H_

#include <avr/pgmspace.h>

#define VCCSTATE SSD1305_SWITCHCAPVCC
#define WIDTH 128
#define HEIGHT 32
#define PAGES 4

#define SSD1305_SETCONTRAST 0x81
#define SSD1305_DISPLAYALLON_RESUME 0xA4
#define SSD1305_DISPLAYALLON 0xA5
#define SSD1305_NORMALDISPLAY 0xA6
#define SSD1305_INVERTDISPLAY 0xA7
#define SSD1305_DISPLAYOFF 0xAE
#define SSD1305_DISPLAYON 0xAF
#define SSD1305_SETDISPLAYOFFSET 0xD3
#define SSD1305_SETCOMPINS 0xDA
#define SSD1305_SETVCOMDETECT 0xDB
#define SSD1305_SETDISPLAYCLOCKDIV 0xD5
#define SSD1305_SETPRECHARGE 0xD9
#define SSD1305_SETMULTIPLEX 0xA8
#define SSD1305_SETLOWCOLUMN 0x00
#define SSD1305_SETHIGHCOLUMN 0x10
#define SSD1305_SETSTARTLINE 0x40
#define SSD1305_MEMORYMODE 0x20
#define SSD1305_COLUMNADDR 0x21
#define SSD1305_PAGEADDR 0x22
#define SSD1305_COMSCANINC 0xC0
#define SSD1305_COMSCANDEC 0xC8
#define SSD1305_SEGREMAP 0xA0
#define SSD1305_CHARGEPUMP 0x8D
#define SSD1305_EXTERNALVCC 0x01
#define SSD1305_SWITCHCAPVCC 0x02

//Scrolling constants
#define SSD1305_ACTIVATE_SCROLL 0x2F
#define SSD1305_DEACTIVATE_SCROLL 0x2E
#define SSD1305_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1305_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1305_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1305_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1305_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A



void SSD1305_begin();
void SSD1305_display(uint8_t* buffer);
void SSD1305_clear(uint8_t* buffer);
void SSD1305_pixel(int x,int y,char color, uint8_t* buffer);
void SSD1305_bitmap(uint8_t x,uint8_t y,const uint8_t *pBmp, uint8_t chWidth, uint8_t chHeight, uint8_t* buffer);
void SSD1305_char1616(uint8_t x,uint8_t y,uint8_t chChar, uint8_t* buffer);
void SSD1305_char3216(uint8_t x, uint8_t y, uint8_t chChar, uint8_t* buffer);
void SSD1305_string(uint8_t x, uint8_t y, const char *pString, uint8_t Size, uint8_t Mode, uint8_t* buffer);
void SPIWrite(uint8_t *buffer, int bufferLength);
void command(uint8_t cmd);
void data(uint8_t data);

extern const uint8_t waveshare_ch[448];
extern const uint8_t Font1612[11][32];
extern const uint8_t Signal816[16];
extern const uint8_t Msg816[16];
extern const uint8_t Bat816[16];
extern const uint8_t Bluetooth88[8];
extern const uint8_t GPRS88[8];
extern const uint8_t Alarm88[8];


#endif
