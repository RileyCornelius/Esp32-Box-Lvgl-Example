#pragma once

#include "Arduino.h"
#include "Print.h"

// Testing if the user has selected the right board in the menu
#ifndef  TT21100_ADDR
#error Please select the right board in the Arduino IDE: ESP32-S3-Box
#endif

#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>

#define ESP32S3BOX_TFTWIDTH  320
#define ESP32S3BOX_TFTHEIGHT 240

#define ST_CMD_DELAY 0x80 // special signifier for command lists

// Some register settings
#define ST7789_MADCTL_BGR 0x08
#define ST7789_MADCTL_MH 0x04

#define ST7789_FRMCTR1 0xB1
#define ST7789_FRMCTR2 0xB2
#define ST7789_FRMCTR3 0xB3
#define ST7789_INVCTR 0xB4
#define ST7789_DISSET5 0xB6

#define ST7789_PWCTR1 0xC0
#define ST7789_PWCTR2 0xC1
#define ST7789_PWCTR3 0xC2
#define ST7789_PWCTR4 0xC3
#define ST7789_PWCTR5 0xC4
#define ST7789_VMCTR1 0xC5

#define ST7789_PWCTR6 0xFC

#define ST7789_GMCTRP1 0xE0
#define ST7789_GMCTRN1 0xE1

#define ST7789_NOP 0x00
#define ST7789_SWRESET 0x01
#define ST7789_RDDID 0x04
#define ST7789_RDDST 0x09

#define ST7789_SLPIN 0x10
#define ST7789_SLPOUT 0x11
#define ST7789_PTLON 0x12
#define ST7789_NORON 0x13

#define ST7789_INVOFF 0x20
#define ST7789_INVON 0x21
#define ST7789_DISPOFF 0x28
#define ST7789_DISPON 0x29
#define ST7789_CASET 0x2A
#define ST7789_RASET 0x2B
#define ST7789_RAMWR 0x2C
#define ST7789_RAMRD 0x2E

#define ST7789_PTLAR 0x30
#define ST7789_TEOFF 0x34
#define ST7789_TEON 0x35
#define ST7789_MADCTL 0x36
#define ST7789_COLMOD 0x3A

#define ST7789_MADCTL_MY 0x80
#define ST7789_MADCTL_MX 0x40
#define ST7789_MADCTL_MV 0x20
#define ST7789_MADCTL_ML 0x10
#define ST7789_MADCTL_RGB 0x00

#define ST7789_RDID1 0xDA
#define ST7789_RDID2 0xDB
#define ST7789_RDID3 0xDC
#define ST7789_RDID4 0xDD

// Some ready-made 16-bit ('RGB565') color settings:
#define ST7789_BLACK 0x0000
#define ST7789_WHITE 0xFFFF
#define ST7789_RED 0xF800
#define ST7789_GREEN 0x07E0
#define ST7789_BLUE 0x001F
#define ST7789_CYAN 0x07FF
#define ST7789_MAGENTA 0xF81F
#define ST7789_YELLOW 0xFFE0
#define ST7789_ORANGE 0xFC00

/// Subclass of SPITFT for ST77xx displays (lots in common!)
class ESP32S3BOX_TFT : public Adafruit_SPITFT {
public:
  ESP32S3BOX_TFT();

  void init(); // ESP32-S3-BOX DISPLAY 
  void setRotation(uint8_t m);
  void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
  void enableDisplay(boolean enable);
  void enableTearing(boolean enable);
  void enableSleep(boolean enable);

protected:
  void begin(uint32_t freq) {} // empty virtual function declaration from Adafruit_SPITFT 
  void displayInit();
};
