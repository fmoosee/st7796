#include <Arduino.h>
#include "display_driver.h"

lv_obj_t* menuScr;

void setup()
{
    Serial.begin(115200);

    // Uma única linha inicializa tudo: ST7796, FT6336, buffers e LVGL
    display_init();

    // --- Construção da Interface Gráfica ---
    menuScr = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(menuScr, lv_color_hex(0x1E1E1E), 0);
    lv_scr_load(menuScr);

    // Botão circular centralizado
    lv_obj_t* btn = lv_obj_create(menuScr);
    lv_obj_set_size(btn, 130, 130);
    lv_obj_set_style_radius(btn, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x007ACC), 0);
    lv_obj_set_style_border_width(btn, 0, 0);
    lv_obj_center(btn);

    // Texto com contador dentro do botão
    lv_obj_t* label = lv_label_create(btn);
    lv_obj_center(label);
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), 0);
    lv_label_set_text(label, "0");

    // Evento de clique
    lv_obj_add_event_cb(btn, [](lv_event_t* e){
        lv_obj_t* target = lv_event_get_target(e);
        lv_obj_t* lbl = lv_obj_get_child(target, 0);
        static int clicks = 0;
        clicks++;
        lv_label_set_text_fmt(lbl, "%d", clicks);
        Serial.printf("Botao pressionado! Cliques: %d\n", clicks);
    }, LV_EVENT_CLICKED, NULL);
}

void loop()
{
    display_update();
    delay(5);
}