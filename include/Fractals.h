#ifndef FRACTALS_H
#define FRACTALS_H
#include "Image.h"
#include "Module.h"
#include <stdbool.h>

void mandelJuliaSet(Image *dst, float x0, float y0, float dx, float a, float bi, bool juliaSet);
void mandelbrot(Image *dst, float x0, float y0, float dx);
void julia(Image *dst, float x0, float y0, float dx);
double perturb(double y, double r, double i);
double average(double a, double b);
double getLength(double x1, double y1, double z1, double x2, double y2, double z2);

#endif // FRACTALS_H
