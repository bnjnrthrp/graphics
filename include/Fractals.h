#ifndef FRACTALS_H
#define FRACTALS_H
#include "Image.h"
#include <stdbool.h>

void mandelJuliaSet(Image *dst, float x0, float y0, float dx, float a, float bi, bool juliaSet);
void mandelbrot(Image *dst, float x0, float y0, float dx);
void julia(Image *dst, float x0, float y0, float dx);

#endif // FRACTALS_H
