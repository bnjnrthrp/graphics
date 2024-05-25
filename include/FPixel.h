#ifndef FPIXEL_H

#define FPIXEL_H
#include "ppmIO.h"
#include "Color.h"
typedef struct
{
    float rgb[3];
} FPixel;

void convert_Pixel(Pixel *pixel, int rows, int cols, FPixel **dest);
void convert_FPixel(FPixel **fpixel, int rows, int cols, Pixel *dest);

#endif // FPIXEL_H
