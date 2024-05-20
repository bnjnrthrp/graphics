#ifndef FRACTALS_H
#define FRACTALS_H
#include "Image.h"

void mandelbrot(Image *dst, float x0, float y0, float dx);
void julia(Image *dst, float x0, float y0, float dx);

#endif // FRACTALS_H
