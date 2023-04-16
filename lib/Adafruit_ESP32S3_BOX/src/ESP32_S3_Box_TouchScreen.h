#pragma once

#include "Arduino.h"

// Testing if the user has selected the right board in the menu
#ifndef  TT21100_ADDR
#error Please select the right board in the Arduino IDE: ESP32-S3-Box
#endif

class TS_Point {
  public:
    TS_Point(void);
    TS_Point(int16_t x, int16_t y, int16_t z);

    bool operator==(TS_Point);
    bool operator!=(TS_Point);

    int16_t x; /*!< X coordinate */
    int16_t y; /*!< Y coordinate */
    int16_t z; /*!< Z coordinate (often used for pressure) */
};

class ESP32S3BOX_TS {
  public:
    ESP32S3BOX_TS(void);
    boolean begin();
    uint8_t touched(void);
    TS_Point getPoint(uint8_t n = 0); // return touch coordinates for the touch 0 or 1

  private:
    uint8_t _readdata[28] = {0}; // 2 points * 10byte +  7 bytes
    uint16_t readMessageLen(void);

    uint8_t touches;
    uint16_t touchX[2], touchY[2], touchPressure[2];
};
