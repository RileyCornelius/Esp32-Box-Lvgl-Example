#include <Arduino.h>
#include "ui/lv_setup.h"
#include "ui/ui.h"
#include "time_func.h"

void setup(void)
{
  Serial.begin(115200);
  lv_begin();
  ui_init();
  time_init();
}

void loop()
{
  lv_handler();
  update_time();
}
