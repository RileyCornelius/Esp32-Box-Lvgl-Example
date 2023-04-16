//This example implements a simple sliding On/Off button. The example
// demonstrates drawing and touch operations.

// Testing if the user has selected the right board in the menu
#ifndef  TT21100_ADDR
#error Please select the right board in the Arduino IDE: ESP32-S3-Box
#endif

#include "ESP32_S3_Box_TFT.h"
#include "ESP32_S3_Box_TouchScreen.h"

ESP32S3BOX_TFT tft = ESP32S3BOX_TFT();
ESP32S3BOX_TS  ts = ESP32S3BOX_TS();

boolean RecordOn = false;

#define FRAME_X 110
#define FRAME_Y 130
#define FRAME_W 200
#define FRAME_H 100

#define REDBUTTON_X FRAME_X
#define REDBUTTON_Y FRAME_Y
#define REDBUTTON_W (FRAME_W/2)
#define REDBUTTON_H FRAME_H

#define GREENBUTTON_X (REDBUTTON_X + REDBUTTON_W)
#define GREENBUTTON_Y FRAME_Y
#define GREENBUTTON_W (FRAME_W/2)
#define GREENBUTTON_H FRAME_H

void drawFrame()
{
  tft.drawRect(FRAME_X, FRAME_Y, FRAME_W, FRAME_H, ST7789_BLACK);
}

void redBtn()
{ 
  tft.fillRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, ST7789_RED);
  tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, ST7789_BLUE);
  drawFrame();
  tft.setCursor(GREENBUTTON_X + 6 , GREENBUTTON_Y + (GREENBUTTON_H/2));
  tft.setTextColor(ST7789_BLACK);
  tft.setTextSize(3);
  tft.println("ON");
  tft.setCursor(GREENBUTTON_X + 6 + 3 , GREENBUTTON_Y + (GREENBUTTON_H/2) + 2);
  tft.setTextColor(ST7789_CYAN);
  tft.setTextSize(3);
  tft.println("ON");
  RecordOn = false;
}

void greenBtn()
{
  tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, ST7789_GREEN);
  tft.fillRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, ST7789_BLUE);
  drawFrame();
  tft.setCursor(REDBUTTON_X + 6 , REDBUTTON_Y + (REDBUTTON_H/2));
  tft.setTextColor(ST7789_BLACK);
  tft.setTextSize(3);
  tft.println("OFF");
  tft.setCursor(REDBUTTON_X + 6 + 3 , REDBUTTON_Y + (REDBUTTON_H/2) + 2);
  tft.setTextColor(ST7789_CYAN);
  tft.setTextSize(3);
  tft.println("OFF");
  RecordOn = true;
}

void setup(void)
{
  Serial.begin(115200);
  tft.init();
  if (!ts.begin()) { 
    Serial.println("Unable to start touchscreen.");
  } 
  else { 
    Serial.println("Touchscreen started."); 
  }

  tft.fillScreen(ST7789_BLACK);
  tft.setRotation(0); 
  redBtn();
}

void loop()
{
  // See if there's any  touch data for us
  if (ts.touched())
  {   
    // Retrieve a point  
    TS_Point p = ts.getPoint(); 
    // flip X around to match the screen.
    p.x = map(p.x, 0, 320, 320, 0);

    if (RecordOn)
    {
      if((p.x > REDBUTTON_X) && (p.x < (REDBUTTON_X + REDBUTTON_W))) {
        if ((p.y > REDBUTTON_Y) && (p.y <= (REDBUTTON_Y + REDBUTTON_H))) {
          Serial.println("Red btn hit"); 
          redBtn();
        }
      }
    }
    else //Record is off (RecordOn == false)
    {
      if((p.x > GREENBUTTON_X) && (p.x < (GREENBUTTON_X + GREENBUTTON_W))) {
        if ((p.y > GREENBUTTON_Y) && (p.y <= (GREENBUTTON_Y + GREENBUTTON_H))) {
          Serial.println("Green btn hit"); 
          greenBtn();
        }
      }
    }

    Serial.println(RecordOn);
  }  
}