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

int second = 0;
int minute = 0;
int hour = 0;
lv_obj_t* menuScr;
LV_FONT_DECLARE(fontClock);

void secClock(lv_timer_t* t){
  lv_obj_t* obj = (lv_obj_t*)t->user_data;
  lv_obj_set_style_text_font(obj, &fontClock, LV_PART_MAIN);
  second++;
  if(second == 60){
    minute++;
    second = 0;
  } else if(minute == 60){
    hour++;
    minute = 0;
  } else if(hour == 24){
    hour = 0;
  }
  static char output[30];
  snprintf(output, sizeof(output), "%02d:%02d:%02d", hour, minute, second);
  lv_label_set_text(obj, output);
}

char labelText[8];
char* options = "Economia\nEsfriar\nPersonalizado";

void setMode(lv_event_t* e){
  lv_obj_t* menu = lv_event_get_target(e);
  int index = lv_dropdown_get_selected(menu);
  switch (index){
    case 0:
    LV_LOG_USER("item 1");
    break;
    case 1:
    LV_LOG_USER("item 2");
    break;
    case 2:
    LV_LOG_USER("item 3");
    break;
  }
}

void createMenu(){
  menuScr = lv_obj_create(NULL);
  lv_scr_load(menuScr);
  lv_obj_t* labelClock = lv_label_create(menuScr);
  lv_obj_align(labelClock, LV_ALIGN_TOP_LEFT, 10, 10);
  lv_obj_set_style_bg_opa(labelClock, 50, LV_PART_MAIN);
  lv_obj_set_style_radius(labelClock, 10, LV_PART_MAIN); 
  lv_timer_t* timerClock = lv_timer_create(secClock, 1000, labelClock);
  lv_obj_t* menu = lv_dropdown_create(menuScr);
  lv_dropdown_set_options(menu, options);
  lv_dropdown_set_selected_highlight(menu, false);
  lv_dropdown_set_text(menu, "Modo");
  lv_obj_align(menu, LV_ALIGN_TOP_RIGHT, -10, 10);
  lv_dropdown_set_dir(menu, LV_DIR_LEFT);
  lv_obj_add_event_cb(menu, setMode, LV_EVENT_VALUE_CHANGED, NULL);
  lv_dropdown_set_symbol(menu, LV_SYMBOL_LEFT);
}


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
  sscanf(__TIME__ + 6, "%02d", &second);
  sscanf(__TIME__ + 3, "%02d", &minute);
  sscanf(__TIME__ , "%02d", &hour);
  createMenu();
}
void loop()
{
  lv_timer_handler(); /* let the GUI do its work */
  delay(10);
}