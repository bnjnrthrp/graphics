#ifndef PPMIO_H

#define PPMIO_H

typedef struct
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
} FPixel;

FPixel *newImage1d(int rows, int cols);

FPixel *readPPM(int *rows, int *cols, int *colors, char *filename);
void writePPM(FPixel *image, int rows, int cols, int colors, char *filename);

unsigned char *readPGM(int *rows, int *cols, int *intensities, char *filename);
void writePGM(unsigned char *image, long rows, long cols, int intensities, char *filename);

#endif
