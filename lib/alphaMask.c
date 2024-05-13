/*
  This is an example of reading and writing an image using the ppmIO.c
  routines.  To manipulate the image, change the pixel color values.

  Bruce A. Maxwell updated 2021-09-05
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/ppmIO.h"
#include "../include/alphaMask.h"

/**
 * This function will determine if the green channel is the dominant (1.5 ratio or more) channel in a pixel
 * @param pixel the pixel in question
 * @returns boolean, true if the green channel is 1.5 greater than the green and blue channels.
 */
bool isGreenScreen(Pixel pixel)
{
  float threshold = 1.4;
  // Converts values of 0 to 1 to remove issues with divide by 0, otherwise keep it the same.
  pixel.r = pixel.r == 0 ? 1 : pixel.r;
  pixel.b = pixel.b == 0 ? 1 : pixel.b;

  return (float)pixel.g / (float)pixel.r >= threshold && (float)pixel.g / (float)pixel.b >= threshold;
};

unsigned char blendColors(unsigned char foreground, unsigned char background, unsigned char mask)
{
  // Convert mask color value to the range [0, 1]
  float alpha = mask / 255;
  unsigned char result = (alpha * (int)foreground) + ((1 - alpha) * (int)background);

  return result;
};

unsigned char __averageReds(Pixel pixels[], long size)
{

  int sum = 0;
  for (int i = 0; i < size; i++)
  {
    sum += pixels[i].r;
  }
  return sum / size;
}
unsigned char __averageGreens(Pixel pixels[], long size)
{

  int sum = 0;
  for (int i = 0; i < size; i++)
  {
    sum += pixels[i].g;
  }
  return sum / size;
}
unsigned char __averageBlues(Pixel pixels[], long size)
{

  int sum = 0;
  for (int i = 0; i < size; i++)
  {
    sum += pixels[i].b;
  }
  return sum / size;
}

unsigned char averagePixel(Pixel pixels[], long size, int channel)
{
  switch (channel)
  {
  case 0:
    return __averageReds(pixels, size);
  case 1:
    return __averageGreens(pixels, size);
  case 2:
    return __averageBlues(pixels, size);
  default:
    return -1;
  }
}

Pixel *scaleImageHalf(Pixel *original, int rows, int cols)
{
  long scaledRows, scaledCols;
  scaledRows = rows / 2;
  scaledCols = cols / 2;
  Pixel *scaled = newImage1d(rows / 2, cols / 2);
  for (long r = 0; r < rows / 2; r++)
  {
    for (long c = 0; c < cols / 2; c++)
    {
      Pixel upperLeft = original[(2 * r * cols) + (2 * c)];
      Pixel upperRight = original[(2 * r * cols) + (2 * c + 1)];
      Pixel lowerLeft = original[(2 * r * cols + cols) + (2 * c)];
      Pixel lowerRight = original[(2 * r * cols + cols) + (2 * c + 1)];

      Pixel orig[] = {upperLeft, upperRight, lowerLeft, lowerRight};
      // check if there are 2 cols available
      scaled[r * scaledCols + c].r = averagePixel(orig, 4, 0);
      scaled[r * scaledCols + c].g = averagePixel(orig, 4, 1);
      scaled[r * scaledCols + c].b = averagePixel(orig, 4, 2);
    }
  }
  return scaled;
}
