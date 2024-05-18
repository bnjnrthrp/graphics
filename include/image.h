#ifndef IMAGE_H

#define IMAGE_H

typedef struct
{
    FPixel **data;
    int rows;
    int cols;
    float a;
    float z;
    unsigned char maxval;
    char[255] filename;
} Image;

#endif // IMAGE_H
