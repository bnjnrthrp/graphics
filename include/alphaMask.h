#ifndef ALPHAMASK_H
#define ALPHAMASK_H
#include <stdbool.h>
#include "ppmIO.h"

bool isGreenScreen(FPixel pixel);
unsigned char blendColors(unsigned char foreground, unsigned char background, unsigned char mask);
FPixel *scaleImageHalf(FPixel *original, int rows, int cols);
unsigned char averagePixel(FPixel pixels[], long size, int channel);
unsigned char __averageReds(FPixel pixels[], long size);
unsigned char __averageGreens(FPixel pixels[], long size);
unsigned char __averageBlues(FPixel pixels[], long size);
#endif // ALPHAMASK_H
