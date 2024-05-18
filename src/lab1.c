/*
  This is an example of reading and writing an image using the ppmIO.c
  routines.  To manipulate the image, change the pixel color values.

  Bruce A. Maxwell updated 2021-09-05
*/

#include <stdio.h>
#include <stdlib.h>
#include "ppmIO.h"

#define USECPP 0

int main(int argc, char *argv[])
{
  FPixel *image;
  int rows, cols, colors;
  long imagesize;
  long i;

  unsigned char temp;

  if (argc < 3)
  {
    printf("Usage: ppmtest <input file> <output file>\n");
    exit(-1);
  }

  /* read in the image */
  image = readPPM(&rows, &cols, &colors, argv[1]);
  if (!image)
  {
    fprintf(stderr, "Unable to read %s\n", argv[1]);
    exit(-1);
  }

  /* calculate the image size */
  imagesize = (long)rows * (long)cols;

  /* mess with the image here  */
  for (i = 0; i < imagesize; i++)
  {
    // This block rotates the RGB values of each pixel, so that the values in RGB rotate
    // The resulting image has the values GBR in the original channels.
    temp = image[i].r;
    image[i].r = image[i].g;
    image[i].g = image[i].b;
    image[i].b = temp;
  }

  /* write out the resulting image */
  writePPM(image, rows, cols, colors /* s/b 255 */, argv[2]);

  /* free the image memory */
#if USECPP
  delete[] image;
#else
  free(image);
#endif

  return (0);
}
