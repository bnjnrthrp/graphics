#ifndef ALPHAMASK_H
#define ALPHAMASK_H
#include <stdbool.h>
#include "ppmIO.h"

bool isGreenScreen(Pixel pixel);
unsigned char blendColors(unsigned char foreground, unsigned char background, unsigned char mask);
Pixel *scaleImageHalf(Pixel *original, int rows, int cols);
unsigned char averagePixel(Pixel pixels[], long size, int channel);
unsigned char __averageReds(Pixel pixels[], long size);
unsigned char __averageGreens(Pixel pixels[], long size);
unsigned char __averageBlues(Pixel pixels[], long size);
#endif // ALPHAMASK_H
