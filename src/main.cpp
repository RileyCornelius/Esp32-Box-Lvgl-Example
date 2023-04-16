#include <Arduino.h>
#include <ui/lv_setup.h>

void setup(void)
{
  lv_begin();
}

void loop()
{
  lv_handler();
}