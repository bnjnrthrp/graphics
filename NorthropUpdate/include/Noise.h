#ifndef NOISE_H

#define NOISE_H

#include "Image.h"

float fade(float t);
float lerp(float t, float a, float b);
float grad(int hash, float x, float y, float z);
float perlin(float x, float y, float z);
void add_noise(Image *dst, int noiseWidth);

#endif // NOISE_H
