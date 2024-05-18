/**
 * This file represents an image, which contains a size (rows and columns), data (pixels), and other
 * various attributes.
 *
 * @author Benji Northrop
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/Image.h"

// Constructors / Deconstructor
Image *image_create(int rows, int cols)
{

    if (rows <= 0 || cols <= 0)
    {
        fprintf(stderr, "invalid rows/cols input (negative)\n");
        return NULL; // early abort if invalid
    }

    // allocate the struct
    Image *src = (Image *)malloc(sizeof(Image));
    if (src == NULL) // null check
    {
        fprintf(stderr, "Error creating the image\n");
        exit(1);
    }

    image_init(src);

    // Set the size to the provided dimensions
    src->rows = rows;
    src->cols = cols;

    return src;
};
void image_init(Image *src)
{
    src->data = NULL;
    src->rows = 0;
    src->cols = 0;
    src->a = 0;
    src->maxval = 1;
    for (int i = 0; i < MAX_FILENAME_LENGTH; i++)
    {
        src->filename[i] = 0;
    };
};
int image_alloc(Image *src, int rows, int cols);

void image_free(Image *src);
void image_dealloc(Image *src);
