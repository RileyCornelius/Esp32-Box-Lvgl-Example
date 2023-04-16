#include "ESP32_S3_Box_TFT.h"

static const uint8_t ST7789_INIT_CMD[] = {
  21,                              // 21 commands in list:
  ST7789_SWRESET,   ST_CMD_DELAY,  //  1: Software reset, 0 args, w/delay
  150,                             //     150 ms delay
  ST7789_SLPOUT,    ST_CMD_DELAY,  //  2: Out of sleep mode, 0 args, w/delay
  255,                             //     500 ms delay
  ST7789_FRMCTR1, 3,               //  3: Framerate ctrl - normal mode, 3 arg:
  0x01, 0x2C, 0x2D,                //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
  ST7789_FRMCTR2, 3,               //  4: Framerate ctrl - idle mode, 3 args:
  0x01, 0x2C, 0x2D,                //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
  ST7789_FRMCTR3, 6,               //  5: Framerate - partial mode, 6 args:
  0x01, 0x2C, 0x2D,                //     Dot inversion mode
  0x01, 0x2C, 0x2D,                //     Line inversion mode
  ST7789_INVCTR,  1,               //  6: Display inversion ctrl, 1 arg:
  0x07,                            //     No inversion
  ST7789_PWCTR1,  3,               //  7: Power control, 3 args, no delay:
  0xA2,
  0x02,                            //     -4.6V
  0x84,                            //     AUTO mode
  ST7789_PWCTR2,  1,               //  8: Power control, 1 arg, no delay:
  0xC5,                            //     VGH25=2.4C VGSEL=-10 VGH=3 * AVDD
  ST7789_PWCTR3,  2,               //  9: Power control, 2 args, no delay:
  0x0A,                            //     Opamp current small
  0x00,                            //     Boost frequency
  ST7789_PWCTR4,  2,               // 10: Power control, 2 args, no delay:
  0x8A,                            //     BCLK/2,
  0x2A,                            //     opamp current small & medium low
  ST7789_PWCTR5,  2,               // 11: Power control, 2 args, no delay:
  0x8A, 0xEE,
  ST7789_VMCTR1,  1,               // 12: Power control, 1 arg, no delay:
  0x0E,
  ST7789_INVOFF,  0,               // 13: Don't invert display, no args
  ST7789_MADCTL,  1,               // 14: Mem access ctl (directions), 1 arg:
  0xC8,                            //     row/col addr, bottom-top refresh
  ST7789_COLMOD,  1,               // 15: set color mode, 1 arg, no delay:
  0x05,                            //     16-bit color
  ST7789_CASET,   4,               // 16: Column addr set, 4 args, no delay:
  0x00, 0x02,                      //     XSTART = 0
  0x00, 0x7F + 0x02,               //     XEND = 127
  ST7789_RASET,   4,               // 17: Row addr set, 4 args, no delay:
  0x00, 0x01,                      //     XSTART = 0
  0x00, 0x9F + 0x01,               //     XEND = 159
  ST7789_GMCTRP1, 16      ,        // 18: Gamma Adjustments (pos. polarity), 16 args + delay:
  0x02, 0x1c, 0x07, 0x12,          //     (Not entirely necessary, but provides
  0x37, 0x32, 0x29, 0x2d,          //      accurate colors)
  0x29, 0x25, 0x2B, 0x39,
  0x00, 0x01, 0x03, 0x10,
  ST7789_GMCTRN1, 16      ,        // 19: Gamma Adjustments (neg. polarity), 16 args + delay:
  0x03, 0x1d, 0x07, 0x06,          //     (Not entirely necessary, but provides
  0x2E, 0x2C, 0x29, 0x2D,          //      accurate colors)
  0x2E, 0x2E, 0x37, 0x3F,
  0x00, 0x00, 0x02, 0x10,
  ST7789_NORON,     ST_CMD_DELAY,  // 20: Normal display on, no args, w/delay
  10,                              //     10 ms delay
  ST7789_DISPON,    ST_CMD_DELAY,  // 21: Main screen turn on, no args w/delay
  100                              //     100 ms delay
};

ESP32S3BOX_TFT::ESP32S3BOX_TFT()
  : Adafruit_SPITFT(ESP32S3BOX_TFTWIDTH, ESP32S3BOX_TFTHEIGHT, -1, TFT_DC, TFT_RST) {

  // Turn LCD Backlight ON
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  // Set SPI to work with the TFT
  SPI.begin(TFT_CLK, TFT_MISO, TFT_MOSI, TFT_CS);
  SPI.setHwCs(true);
}

void ESP32S3BOX_TFT::init() {
  // base class setup
  invertOnCommand = ST7789_INVON;
  invertOffCommand = ST7789_INVOFF;
  
  // initialization
  initSPI(40000000, SPI_MODE0);  // 40Mhz SPI
  displayInit();
}

void ESP32S3BOX_TFT::displayInit() {

  uint8_t numCommands, cmd, numArgs;
  uint16_t ms;

  const uint8_t *addr = ST7789_INIT_CMD;
  numCommands = *addr++;               // Number of commands to follow
  while (numCommands--) {              // For each command...
    cmd = *addr++;                     // Read command
    numArgs = *addr++;                 // Number of args to follow
    ms = numArgs & ST_CMD_DELAY;       // If hibit set, delay follows args
    numArgs &= ~ST_CMD_DELAY;          // Mask out delay bit
    sendCommand(cmd, addr, numArgs);
    addr += numArgs;

    if (ms) {
      ms = *addr++;                    // Read post-command delay time (ms)
      if (ms == 255)
        ms = 500;                      // If 255, delay for 500 ms
      delay(ms);
    }
  }
}

void ESP32S3BOX_TFT::setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
  x += _xstart;
  y += _ystart;
  uint32_t xa = ((uint32_t)x << 16) | (x + w - 1);
  uint32_t ya = ((uint32_t)y << 16) | (y + h - 1);

  writeCommand(ST7789_CASET); // Column addr set
  SPI_WRITE32(xa);

  writeCommand(ST7789_RASET); // Row addr set
  SPI_WRITE32(ya);

  writeCommand(ST7789_RAMWR); // write to RAM
}

void ESP32S3BOX_TFT::enableDisplay(boolean enable) {
  sendCommand(enable ? ST7789_DISPON : ST7789_DISPOFF);
}

void ESP32S3BOX_TFT::enableTearing(boolean enable) {
  sendCommand(enable ? ST7789_TEON : ST7789_TEOFF);
}

void ESP32S3BOX_TFT::enableSleep(boolean enable) {
  sendCommand(enable ? ST7789_SLPIN : ST7789_SLPOUT);
}

void ESP32S3BOX_TFT::setRotation(uint8_t m) {
  uint8_t madctl = 0;

  rotation = m & 3; // can't be higher than 3

  switch (rotation) {
    case 0:
      madctl = ST7789_MADCTL_MX | ST7789_MADCTL_MY | ST7789_MADCTL_BGR;

      _height = ESP32S3BOX_TFTHEIGHT;
      _width = ESP32S3BOX_TFTWIDTH;
      _xstart = 0;
      _ystart = 0;
      break;
    case 1:
      madctl = ST7789_MADCTL_MY | ST7789_MADCTL_MV | ST7789_MADCTL_BGR;

      _width = ESP32S3BOX_TFTHEIGHT;
      _height = ESP32S3BOX_TFTWIDTH;
      _ystart = 0;
      _xstart = 0;
      break;
    case 2:
      madctl = ST7789_MADCTL_BGR;

      _height = ESP32S3BOX_TFTHEIGHT;
      _width = ESP32S3BOX_TFTWIDTH;
      _xstart = 0;
      _ystart = 0;
      break;
    case 3:
      madctl = ST7789_MADCTL_MX | ST7789_MADCTL_MV | ST7789_MADCTL_BGR;

      _width = ESP32S3BOX_TFTHEIGHT;
      _height = ESP32S3BOX_TFTWIDTH;
      _ystart = 0;
      _xstart = 0;
      break;
  }

  sendCommand(ST7789_MADCTL, &madctl, 1);
}
