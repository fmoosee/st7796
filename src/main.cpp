#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include "touch.h"
#include <fontClock.c>

/*Don't forget to set Sketchbook location in File/Preferencesto the path of your UI project (the parent foder of this INO file)*/

/*Change to your screen resolution*/
static const uint16_t screenWidth  = 480;
static const uint16_t screenHeight = 320;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * screenHeight / 10 ];
TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */


/* Display flushing */
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
  uint32_t w = ( area->x2 - area->x1 + 1 );
  uint32_t h = ( area->y2 - area->y1 + 1 );
  tft.startWrite();
  tft.setAddrWindow( area->x1, area->y1, w, h );
  tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
  tft.endWrite();
  lv_disp_flush_ready( disp );
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  if (touch_has_signal())
  {
    if (touch_touched())
    {
      data->state = LV_INDEV_STATE_PR;
      /*Set the coordinates*/
      data->point.x = touch_last_x;
      data->point.y = touch_last_y;
    }
    else if (touch_released())
    {
      data->state = LV_INDEV_STATE_REL;
    }
  }
  else
  {
    data->state = LV_INDEV_STATE_REL;
  }
}

LV_FONT_DECLARE(fontClock);
lv_obj_t* menuScr;

void setup()
{
  Serial.begin(115200); /* prepare for possible serial debug */
  tft.begin();          /* TFT init */
  tft.setRotation(3); /* Landscape orientation, flipped */
  touch_init(480, 320, 3);
  lv_init();
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * screenHeight / 10);
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init( &indev_drv );
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);
  menuScr = lv_obj_create(NULL);
  lv_scr_load(menuScr);
  lv_obj_t* calendar = lv_calendar_create(menuScr);
  lv_calendar_set_showed_date(calendar, 2025, 2);
  lv_obj_center(calendar);
  lv_calendar_header_arrow_create(calendar);
  lv_obj_add_event_cb(calendar, [](lv_event_t* e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_current_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        lv_calendar_date_t date;
        if(lv_calendar_get_pressed_date(obj, &date)) {
            LV_LOG_USER("Clicked date: %02d.%02d.%d", date.day, date.month, date.year);
            lv_calendar_set_today_date(obj, date.year, date.month, date.day);
        }
    }
  }, LV_EVENT_VALUE_CHANGED, NULL);
}

void loop()
{
  lv_timer_handler(); /* let the GUI do its work */
  delay(10);
}