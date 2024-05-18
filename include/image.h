#ifndef IMAGE_H

#define IMAGE_H
#include "FPixel.h"
#define MAX_FILENAME_LENGTH 255

typedef struct
{
    FPixel **data;
    int rows;
    int cols;
    float a;
    float z;
    float maxval;
    char filename[MAX_FILENAME_LENGTH];
} Image;

// Constructors / Deconstructor
Image *image_create(int rows, int cols);
void image_free(Image *src);
void image_init(Image *src);
int image_alloc(Image *src, int rows, int cols);
void image_dealloc(Image *src);

// input/output
Image *image_read(char *filename);
int image_write(Image *src, char *filename);

// Access
FPixel image_getf(Image *src, int r, int c);
float image_getc(Image *src, int r, int c, int b);
float image_geta(Image *src, int r, int c);
float image_getz(Image *src, int r, int c);
void image_setf(Image *src, int r, int c, FPixel val);
void image_setc(Image *src, int r, int c, int b, float val);
void image_seta(Image *src, int r, int c, float val);
void image_setz(Image *src, int r, int c, float val);

// Utility
void image_reset(Image *src);
void image_fill(Image *src, FPixel val);
void image_fillrgb(Image *src, float r, float g, float b);
void image_filla(Image *src, float a);
void image_fillz(Image *src, float z);

#endif // IMAGE_H
