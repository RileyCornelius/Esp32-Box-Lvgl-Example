#include <Arduino.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include <ESP32_S3_Box_TouchScreen.h>

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library
ESP32S3BOX_TS ts = ESP32S3BOX_TS();

void setup(void)
{
  Serial.begin(115200);
  ts.begin();
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("Hello World", 0, 0, 2);
}

void loop()
{
  uint16_t x, y;
  if (ts.touched())
  {
    // Retrieve a point
    TS_Point p = ts.getPoint();
    Serial.println("X = " + String(p.x) + " Y = " + String(p.y));
  }
}