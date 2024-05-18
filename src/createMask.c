/**
 * This program will take an image with a green screen background
 * and create an alpha mask so it may be composited with another
 * image.
 *
 * @author Benji Northrop
 * CS5310
 * Summer 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include "../include/ppmIO.h"
#include "../include/alphaMask.h"

int main(int argc, char *argv[])
{
    FPixel *image;
    int rows, cols, colors;
    long imagesize;
    long i;

    if (argc < 3)
    {
        printf("Usage: alphaMask <input file> <output file>\n");
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

    /* create the alpha mask, but converting the blue/green screen to 0 and 255 for foreground.  */
    for (i = 0; i < imagesize; i++)
    {

        // this piece converts the green/blue screen to black (background) and the remaining
        // to white (foreground)
        if (isGreenScreen(image[i]))
        {
            image[i].r = 0;
            image[i].g = 0;
            image[i].b = 0;
        }
        else
        {
            image[i].r = 255;
            image[i].g = 255;
            image[i].b = 255;
        }
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
