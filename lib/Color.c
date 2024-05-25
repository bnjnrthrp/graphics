/**
 * This file represents an color, which contains a red, blue, and green channel
 *
 * @author Benji Northrop
 */

#include <stdio.h>
#include <stdlib.h>
#include "Color.h"
#include "ppmIO.h"

void color_copy(Color *to, Color *from)
{

    to->c[0] = from->c[0];
    to->c[1] = from->c[1];
    to->c[2] = from->c[2];
}
void color_set(Color *to, float r, float g, float b)
{
    // check to see if any parameters are outside limits, and skip rest of checks if its ok.
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

    to->c[0] = r;
    to->c[1] = g;
    to->c[2] = b;
}

float uc_to_float(unsigned char val)
{
    float result = val / 255;
    return result;
}

unsigned char float_to_uc(float val)
{
    unsigned char result = val * 255;
    return result;
}

Color color_from_pixel(Pixel pixel)
{
    Color color;
    color.c[0] = uc_to_float(pixel.r);
    color.c[1] = uc_to_float(pixel.g);
    color.c[2] = uc_to_float(pixel.b);
    return color;
}

Pixel pixel_from_color(Color color)
{
    Pixel pixel;
    pixel.r = float_to_uc(color.c[0]);
    pixel.g = float_to_uc(color.c[1]);
    pixel.b = float_to_uc(color.c[2]);
    return pixel;
}
