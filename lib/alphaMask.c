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
  float threshold = 1.5;
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
