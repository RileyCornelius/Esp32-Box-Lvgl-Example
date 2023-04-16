#include "Arduino.h"
#include "ESP32_S3_Box_TouchScreen.h"
#include <Wire.h>

ESP32S3BOX_TS::ESP32S3BOX_TS() {
  // reset all information
  touchX[0] = touchY[0] = touchPressure[0] = 0;
  touchX[1] = touchY[1] = touchPressure[1] = 0;
  touches = 0;
}

boolean ESP32S3BOX_TS::begin() {
  Wire1.begin(I2C_SDA, I2C_SCL, 400000);

  uint8_t tries = 5;
  do {
    readMessageLen();
    tries--;
  } while (_readdata[0] == 0 && tries > 0);
  return _readdata[0] == 2;
}

uint16_t ESP32S3BOX_TS::readMessageLen(void) {

  _readdata[0] = 0;
  _readdata[1] = 0;
  Wire1.beginTransmission(TT21100_ADDR);
  Wire1.endTransmission();
  Wire1.requestFrom((byte)TT21100_ADDR, (byte)2);
  _readdata[0] = Wire1.read();
  _readdata[1] = Wire1.read();
  return _readdata[0] + (_readdata[1] << 8);
}

// reads sensor and touches
// updates Touch Points, but if not touched, resets all Touch Point Information
uint8_t ESP32S3BOX_TS::touched(void) {
  // reset all information
  touchX[0] = touchY[0] = touchPressure[0] = 0;
  touchX[1] = touchY[1] = touchPressure[1] = 0;
  touches = 0;
  
  // was it touched?
  uint16_t msgLen = readMessageLen();

  if (msgLen == 7 || msgLen == 17 || msgLen == 27 || msgLen == 14) {
    // read actual data
    Wire1.beginTransmission(TT21100_ADDR);
    Wire1.endTransmission();
    Wire1.requestFrom((byte)TT21100_ADDR, (byte)msgLen);
    for (uint8_t i = 0; i < msgLen; i++) {
      _readdata[i] = Wire1.read();
    }

    // check that it is not the touch button event of ESP32BOX (little red circle)
    if (msgLen != 14) {
    
      // set touch information
      touches = msgLen / 10;
      if (touches == 1) {
        touchX[0] = _readdata[9] + (_readdata[10] << 8);
        touchY[0] = _readdata[11] + (_readdata[12] << 8);
        touchPressure[0] = _readdata[13];
      }
      if (touches == 2) {
        touchX[0] = _readdata[19] + (_readdata[20] << 8);
        touchY[0] = _readdata[21] + (_readdata[22] << 8);
        touchPressure[0] = _readdata[23];
      }
    } // for now just ignore that button...
  }
  

  return touches;
}

// Returns last Touched Coordinates got using touched()
TS_Point ESP32S3BOX_TS::getPoint(uint8_t n) {

  if ((touches == 0) || (n > 1)) {
    return TS_Point(0, 0, 0);
  } else {
    return TS_Point(touchX[n], touchY[n], touchPressure[n]);
  }
}

TS_Point::TS_Point(void) {
  x = y = p = 0;
}

TS_Point::TS_Point(int16_t _x, int16_t _y, int16_t _z) {
  x = _x;
  y = _y;
  p = _z;
}

bool TS_Point::operator==(TS_Point p1) {
  return ((p1.x == x) && (p1.y == y));
}

bool TS_Point::operator!=(TS_Point p1) {
  return ((p1.x != x) || (p1.y != y));
}
