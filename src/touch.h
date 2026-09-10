#ifndef TOUCH_H
#define TOUCH_H

#include <FT6336.h>

#define TOUCH_FT6336_SCL 22
#define TOUCH_FT6336_SDA 21
#define TOUCH_FT6336_INT 14
#define TOUCH_FT6336_RST 12

int touch_last_x = 0;
int touch_last_y = 0;
uint16_t touch_width = 0;
uint16_t touch_height = 0;

// O chip nativo lê o painel em 320x480
FT6336 ts = FT6336(TOUCH_FT6336_SDA, TOUCH_FT6336_SCL, TOUCH_FT6336_INT, TOUCH_FT6336_RST, 320, 480);

void touch_init(uint16_t w, uint16_t h, uint8_t r)
{
    touch_width = w;
    touch_height = h;
    ts.begin();
    ts.setRotation(r);
}

bool touch_touched(void)
{
    ts.read();
    if (ts.isTouched)
    {
        // Limita as coordenadas lidas aos limites da tela para evitar avisos no LVGL
        touch_last_x = constrain(ts.points[0].x, 0, touch_width - 1);
        touch_last_y = constrain(ts.points[0].y, 0, touch_height - 1);
        return true;
    }
    return false;
}

bool touch_has_signal(void)
{
    return true;
}

bool touch_released(void)
{
    return !ts.isTouched;
}

#endif