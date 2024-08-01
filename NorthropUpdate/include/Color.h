#ifndef COLOR_H
#define COLOR_H
#include "ppmIO.h"

typedef struct Color
{
    float c[3];
} Color;

void color_copy(Color *to, Color *from);
void color_set(Color *to, float r, float g, float b);
float uc_to_float(unsigned char val);
unsigned char float_to_uc(float val);
Color color_from_pixel(Pixel pixel);
Pixel pixel_from_color(Color color);
#endif // COLOR_H
