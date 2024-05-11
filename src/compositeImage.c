/**
 * This program will a background image, a green screen image, and a mask, and combine
 * them to create a composite image.
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
    Pixel *bgImage, *fgImage, *mask, *blendedImage; // background image, foreground image, and mask
    int bgRows, bgCols, bgColors, fgRows, fgCols, fgColors, maskRows, maskCols, maskColors;
    long bgImageSize, fgImageSize;
    long i;

    if (argc < 5)
    {
        printf("Usage: alphaMask <input backgroundImage> <input foregroundImage> <input mask> <output file>\n");
        exit(-1);
    }

    /* read in the background */
    bgImage = readPPM(&bgRows, &bgCols, &bgColors, argv[1]);
    if (!bgImage)
    {
        fprintf(stderr, "Unable to read %s\n", argv[1]);
        exit(-1);
    }

    /* read in the foreground */
    fgImage = readPPM(&fgRows, &fgCols, &fgColors, argv[2]);
    if (!fgImage)
    {
        fprintf(stderr, "Unable to read %s\n", argv[2]);
        exit(-1);
    }

    /* read in the mask */
    mask = readPPM(&maskRows, &maskCols, &maskColors, argv[3]);
    if (!mask)
    {
        fprintf(stderr, "Unable to read %s\n", argv[3]);
        exit(-1);
    }

    /* calculate the output image size */
    bgImageSize = (long)bgRows * (long)bgCols;
    fgImageSize = (long)fgRows * (long)fgCols;

    blendedImage = bgImage; // new pointer to original background image
    /* loop through the background and mask, adding the foreground image where the mask allows it  */
    for (i = 0; i < bgImageSize; i++)
    {
        // Ensure i is within the boundaries of the foregound image
        if (i < fgImageSize)
        {
            /// Convert everything to [i]
            blendedImage[i].r = blendColors(fgImage[i].r, bgImage[i].r, mask[i].r);
            blendedImage[i].g = blendColors(fgImage[i].g, bgImage[i].g, mask[i].g);
            blendedImage[i].b = blendColors(fgImage[i].b, bgImage[i].b, mask[i].b);
        }
    }

    /* write out the resulting image */
    writePPM(blendedImage, bgRows, bgCols, bgColors /* s/b 255 */, argv[4]);

    /* free the image memory */

    free(bgImage);
    free(fgImage);
    free(mask);

    return (0);
}
