
#include <Arduino.h>
#include <lvgl.h>

#define LGFX_AUTODETECT // Automatically detect the display of esp32-box from Arduino defines
#include <LovyanGFX.hpp>

/* Esp32-box screen size */
#define LV_SCREEN_WIDTH 320
#define LV_SCREEN_HEIGHT 240
#define LV_BUF_SIZE (LV_SCREEN_WIDTH * LV_SCREEN_HEIGHT)

LGFX tft; // Touch screen object

/* Display flushing */
static void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)&color_p->full, w * h);

    lv_disp_flush_ready(disp);
}

/* Read the touchpad */
static void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    uint16_t x, y;
    bool touched = tft.getTouch(&x, &y);
    if (touched)
    {
        data->point.x = x;
        data->point.y = y;
        data->state = LV_INDEV_STATE_PR;
        // Serial.println("X = " + String(x) + " Y = " + String(y));
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }
}

/* Setup lvgl with display and touch pad */
void lv_begin()
{
    /*Initialize the display and touch driver*/
    tft.init();

    /* Call before other lv functions */
    lv_init();

    /* Initialize the display buffers */
    static lv_disp_draw_buf_t draw_buf;
    static lv_color_t *buf1, *buf2;
    buf1 = (lv_color_t *)heap_caps_malloc(LV_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);
    assert(buf1);
    buf2 = (lv_color_t *)heap_caps_malloc(LV_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);
    assert(buf2);
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, LV_BUF_SIZE);

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = LV_SCREEN_WIDTH;
    disp_drv.ver_res = LV_SCREEN_HEIGHT;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    /*Initialize the input device driver*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);
}

/* Handles updating the display and touch events */
void lv_handler()
{
    static uint32_t previousUpdate = 0;
    static uint32_t interval = 0;

    if (millis() - previousUpdate > interval)
    {
        previousUpdate = millis();
        interval = lv_timer_handler(); // Update the UI
        // Serial.println(interval);
    }
}
