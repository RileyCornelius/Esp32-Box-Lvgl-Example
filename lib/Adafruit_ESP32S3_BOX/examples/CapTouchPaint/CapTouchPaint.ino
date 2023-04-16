/*
   This sketch targets the ESP32-S3-BOX board ONLY
   It demonstrates a compatible Touch Screen sketch
*/

// Testing if the user has selected the right board in the menu
#ifndef  TT21100_ADDR
#error Please select the right board in the Arduino IDE: ESP32-S3-Box
#endif

#include "ESP32_S3_Box_TFT.h"
#include "ESP32_S3_Box_TouchScreen.h"

ESP32S3BOX_TFT tft = ESP32S3BOX_TFT();
ESP32S3BOX_TS  ts = ESP32S3BOX_TS();

// Size of the color selection boxes and the paintbrush size
#define BOXSIZE 40
#define PENRADIUS 3
int oldcolor, currentcolor;

void setup(void) {
  Serial.begin(115200);
  delay(500);
  
  Serial.println("Cap Touch Paint!");
  
  tft.init();

  if (! ts.begin()) {
    Serial.println("Couldn't start TT21100 touchscreen controller");
    while (1);
  }

  Serial.println("Capacitive touchscreen started");
  
  tft.fillScreen(ST7789_BLACK);
  
  // make the color selection boxes
  tft.fillRect(0, 0, BOXSIZE, BOXSIZE, ST7789_RED);
  tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, ST7789_YELLOW);
  tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, ST7789_GREEN);
  tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, ST7789_CYAN);
  tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, ST7789_BLUE);
  tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, ST7789_MAGENTA);
 
  // select the current color 'red'
  tft.drawRect(0, 0, BOXSIZE, BOXSIZE, ST7789_WHITE);
  currentcolor = ST7789_RED;
}

void loop() {
  // Wait for a touch
  if (! ts.touched()) {
    return;
  }

  // Retrieve a point  
  TS_Point p = ts.getPoint();
  
 
  // Print out raw data from screen touch controller
  Serial.print("X = "); Serial.print(p.x);
  Serial.print("\tY = "); Serial.print(p.y);
  Serial.print(" -> ");
 

  // flip X around to match the screen.
  p.x = map(p.x, 0, 320, 320, 0);

  // Print out the remapped (rotated) coordinates
  Serial.print("("); Serial.print(p.x);
  Serial.print(", "); Serial.print(p.y);
  Serial.println(")");
  

  if (p.y < BOXSIZE) {
     oldcolor = currentcolor;

     if (p.x < BOXSIZE) { 
       currentcolor = ST7789_RED; 
       tft.drawRect(0, 0, BOXSIZE, BOXSIZE, ST7789_WHITE);
     } else if (p.x < BOXSIZE*2) {
       currentcolor = ST7789_YELLOW;
       tft.drawRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, ST7789_WHITE);
     } else if (p.x < BOXSIZE*3) {
       currentcolor = ST7789_GREEN;
       tft.drawRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, ST7789_WHITE);
     } else if (p.x < BOXSIZE*4) {
       currentcolor = ST7789_CYAN;
       tft.drawRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, ST7789_WHITE);
     } else if (p.x < BOXSIZE*5) {
       currentcolor = ST7789_BLUE;
       tft.drawRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, ST7789_WHITE);
     } else if (p.x <= BOXSIZE*6) {
       currentcolor = ST7789_MAGENTA;
       tft.drawRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, ST7789_WHITE);
     }

     if (oldcolor != currentcolor) {
        if (oldcolor == ST7789_RED) 
          tft.fillRect(0, 0, BOXSIZE, BOXSIZE, ST7789_RED);
        if (oldcolor == ST7789_YELLOW) 
          tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, ST7789_YELLOW);
        if (oldcolor == ST7789_GREEN) 
          tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, ST7789_GREEN);
        if (oldcolor == ST7789_CYAN) 
          tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, ST7789_CYAN);
        if (oldcolor == ST7789_BLUE) 
          tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, ST7789_BLUE);
        if (oldcolor == ST7789_MAGENTA) 
          tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, ST7789_MAGENTA);
     }
  }
  if (((p.y-PENRADIUS) > BOXSIZE) && ((p.y+PENRADIUS) < tft.height())) {
    tft.fillCircle(p.x, p.y, PENRADIUS, currentcolor);
  }
}