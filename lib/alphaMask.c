/*
  This file has utility functions for creating an alpha mask from a green screen image
  and scaling an image down.

  @author Benji Northrop
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/ppmIO.h"
#include "../include/alphaMask.h"

/**
 * This function will determine if the green channel is the dominant channel in a pixel
 * @param pixel the pixel in question
 * @returns boolean, true if the green channel is 1.5 greater than the green and blue channels.
 */
bool isGreenScreen(FPixel pixel)
{
  float threshold = 1.3;
  // Converts values of 0 to 1 to remove issues with divide by 0, otherwise keep it the same.
  pixel.r = pixel.r == 0 ? 1 : pixel.r;
  pixel.b = pixel.b == 0 ? 1 : pixel.b;

  // returns true under 3 criteria: green is > 50, and the ratio between green and the others is >1.3

  return pixel.g > 50 && (float)pixel.g / (float)pixel.r >= threshold && (float)pixel.g / (float)pixel.b >= threshold;
};

/**
 * Blends the colors together using an alpha mask to composite a foreground and
 * background image using a(foreground) + (1-a)(background). Works in one channel only.
 *
 * @param foreground the foreground pixel value (r, g, or b)
 * @param backround the background pixel value
 * @param mask the alpha mask value
 * @return the value of blended pixel
 */
unsigned char blendColors(unsigned char foreground, unsigned char background, unsigned char mask)
{
  // Convert mask color value to the range [0, 1]
  float alpha = mask / 255;
  unsigned char result = (alpha * (int)foreground) + ((1 - alpha) * (int)background);

  return result;
};

/**
 * Helper function that averages the red channels of an array of pixels.
 * Only does a balanced average
 * @param pixels the array of pixels to average
 * @param size the size of the array
 * @return unsigned char of the average size of the pixel
 */
unsigned char __averageReds(FPixel pixels[], long size)
{

  int sum = 0;
  for (int i = 0; i < size; i++)
  {
    sum += pixels[i].r;
  }
  return sum / size;
}

/**
 * Helper function that averages the green channels of an array of pixels.
 * Only does a balanced average
 * @param pixels the array of pixels to average
 * @param size the size of the array
 * @return unsigned char of the average size of the pixel
 */
unsigned char __averageGreens(FPixel pixels[], long size)
{

  int sum = 0;
  for (int i = 0; i < size; i++)
  {
    sum += pixels[i].g;
  }
  return sum / size;
}

/**
 * Helper function that averages the blue channels of an array of pixels.
 * Only does a balanced average
 * @param pixels the array of pixels to average
 * @param size the size of the array
 * @return unsigned char of the average size of the pixel
 */
unsigned char __averageBlues(FPixel pixels[], long size)
{

  int sum = 0;
  for (int i = 0; i < size; i++)
  {
    sum += pixels[i].b;
  }
  return sum / size;
}

/**
 * Helper function to determine the average value of a channel in a pixel.
 * @param pixels[] an array of the pixels to average
 * @param size the size of the pixel array
 * @param channel the channel to average. 0 = red, 1 = green, 2 = blue
 * @return the average value of that pixel in that channel
 */
unsigned char averagePixel(FPixel pixels[], long size, int channel)
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

/**
 * Scales an image down to 25% the original by reducing each dimension by 50%
 * @param original the original image
 * @param rows the number of rows of the original image
 * @param cols the number of columns of the original image
 * @return a pointer to the scaled pixel image
 */
FPixel *scaleImageHalf(FPixel *original, int rows, int cols)
{
  long scaledRows, scaledCols;
  scaledRows = rows / 2;
  scaledCols = cols / 2;
  FPixel *scaled = newImage1d(scaledRows, scaledCols);
  for (long r = 0; r < rows / 2; r++)
  {
    for (long c = 0; c < cols / 2; c++)
    {
      FPixel upperLeft = original[(2 * r * cols) + (2 * c)];
      FPixel upperRight = original[(2 * r * cols) + (2 * c + 1)];
      FPixel lowerLeft = original[(2 * r * cols + cols) + (2 * c)];
      FPixel lowerRight = original[(2 * r * cols + cols) + (2 * c + 1)];

      FPixel orig[] = {upperLeft, upperRight, lowerLeft, lowerRight};
      // check if there are 2 cols available
      scaled[r * scaledCols + c].r = averagePixel(orig, 4, 0);
      scaled[r * scaledCols + c].g = averagePixel(orig, 4, 1);
      scaled[r * scaledCols + c].b = averagePixel(orig, 4, 2);
    }
  }
  return scaled;
}
