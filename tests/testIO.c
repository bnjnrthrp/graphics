/*
  This is an example of reading and writing an image using the ppmIO.c
  routines.  To manipulate the image, change the pixel color values.

  Bruce A. Maxwell updated 2021-09-05
*/

#include <stdio.h>
#include <stdlib.h>
#include "ppmIO.h"
#include "Image.h"

#define USECPP 0

int main(int argc, char *argv[])
{
  Image *image;
  int rows, cols, colors;
  long imagesize;
  long i, j;
  int min;

  if (argc < 3)
  {
    printf("Usage: ppmtest <input file> <output file>\n");
    exit(-1);
  }

  /* read in the image */
  image = image_read(argv[1]);
  if (!image)
  {
    fprintf(stderr, "Unable to read %s\n", argv[1]);
    exit(-1);
  }

  /* calculate the image size */
  imagesize = (long)rows * (long)cols;

  /* write out the resulting image */
  image_write(image, argv[2]);

  /* free the image memory */
#if USECPP
  delete[] image;
#else
  free(image);
#endif

  return (0);
}
