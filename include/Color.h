#ifndef COLOR_H
#define COLOR_H
#include "Image.h"
#include "ppmIO.h"

typedef struct
{
    float c[3];
} Color;

void color_copy(Color *to, Color *from);
void color_set(Color *to, float r, float g, float b);
void image_setColor(Image *src, int r, int c, Color val);
Color image_getColor(Image *src, int r, int c);
float int_to_float(int val);
int float_to_int(float val);
Color color_from_pixel(Pixel pixel);
Pixel pixel_from_color(Color color);
#endif // COLOR_H
