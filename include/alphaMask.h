#ifndef ALPHAMASK_H
#define ALPHAMASK_H
#include <stdbool.h>
#include "ppmIO.h"

bool isGreenScreen(Pixel pixel);
unsigned char blendColors(unsigned char foreground, unsigned char background, unsigned char mask);

#endif // ALPHAMASK_H
