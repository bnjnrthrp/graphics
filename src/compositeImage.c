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
    Pixel *bgImage, *fgImage, *mask, *scaled; // background image, foreground image, and mask
    int bgRows, bgCols, bgColors, fgRows, fgCols, fgColors, maskRows, maskCols, maskColors;
    long fgImageSize, dx, dy;

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
    fgImageSize = (long)fgRows * (long)fgCols;

    /**
     * TODO: Add scale function to scale the foreground image down by integer amount
     */

    long j = 0; // index tracker for image 2
    dx = 400;
    dy = 80;
    for (long r = 0; r < bgRows; r++)
    {
        for (long c = 0; c < bgCols; c++)
            // If the background image index is at the offset of the foreground image, start compositing images
            // This allows for a smaller foreground dimension
            if (c >= dx && c < fgCols + dx && r >= dy && r < fgRows + dy)
            {
                /// Blend each channel at i (the background), and j (foreground)
                bgImage[r * bgCols + c].r = blendColors(fgImage[j].r, bgImage[(r)*bgCols + c].r, mask[j].r);
                bgImage[r * bgCols + c].g = blendColors(fgImage[j].g, bgImage[(r)*bgCols + c].g, mask[j].g);
                bgImage[r * bgCols + c].b = blendColors(fgImage[j].b, bgImage[(r)*bgCols + c].b, mask[j].b);
                j++; // Increment the foreground
            }
    }

    /* write out the resulting image */
    writePPM(bgImage, bgRows, bgCols, bgColors /* s/b 255 */, argv[4]);

    /* free the image memory */

    free(bgImage);
    free(fgImage);
    free(mask);

    return (0);
}
