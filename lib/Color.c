/**
 * This file represents an color, which contains a red, blue, and green channel
 *
 * @author Benji Northrop
 */

#include <stdio.h>
#include <stdlib.h>
#include "Image.h"
#include "Color.h"
#include "ppmIO.h"

void color_copy(Color *to, Color *from)
{
    // Iterate through each channel in the color array.
    for (int i = 0; i < 3; i++)
    {
        to->c[i] = from->c[i];
    }
}
void color_set(Color *to, float r, float g, float b)
{
    // Do one check to see if any parameters are outside limits, and skip rest of checks if its ok.
    if (r < 0 || g < 0 || b < 0 || r > 1 || g > 1 || b > 1)
    {
        if (r < 0)
        {
            r = 0;
        }
        else if (r > 1)
        {
            r = 1;
        }
        if (g < 0)
        {
            g = 0;
        }
        else if (g > 1)
        {
            g = 1;
        }
        if (b < 0)
        {
            b = 0;
        }
        else if (b > 1)
        {
            b = 1;
        }
    }
    to->c[0] = r;
    to->c[1] = g;
    to->c[2] = b;
}
void image_setColor(Image *src, int r, int c, Color val)
{
    src->data[r][c].rgb[0] = val.c[0];
    src->data[r][c].rgb[1] = val.c[1];
    src->data[r][c].rgb[2] = val.c[2];
}
Color image_getColor(Image *src, int r, int c)
{
    Color color;
    color.c[0] = src->data[r][c].rgb[0];
    color.c[1] = src->data[r][c].rgb[1];
    color.c[2] = src->data[r][c].rgb[2];
    return color;
}

float int_to_float(int val)
{
    return (float)(val / 255);
}

int float_to_int(float val)
{
    return (int)val * 255;
}

Color color_from_pixel(Pixel pixel)
{
    Color color;
    color.c[0] = int_to_float(pixel.r);
    color.c[1] = int_to_float(pixel.g);
    color.c[2] = int_to_float(pixel.b);
    return color;
}

Pixel pixel_from_color(Color color)
{
    Pixel pixel;
    pixel.r = float_to_int(color.c[0]);
    pixel.g = float_to_int(color.c[1]);
    pixel.b = float_to_int(color.c[2]);
    return pixel;
}
