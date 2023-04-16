#include <Arduino.h>
#include <ui/lv_setup.h>
#include <ui/ui.h>

void setup(void)
{
  Serial.begin(115200);
  lv_begin();
  ui_init();
}

void loop()
{
  lv_handler();
}
