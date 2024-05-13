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
    Pixel *bgImage, *fgImage, *mask, *scaled, *scaledMask; // background image, foreground image, and mask
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

    // Scales the foreground image, making it 25% of the original
    scaled = scaleImageHalf(fgImage, fgRows, fgCols);
    long scaledRows = fgRows / 2;
    long scaledCols = fgCols / 2;

    scaledMask = scaleImageHalf(mask, fgRows, fgCols);

    long j = 0; // index tracker for image 2
    dx = 520;
    dy = 80;
    for (long r = 0; r < bgRows; r++)
    {
        for (long c = 0; c < bgCols; c++)
            // If the background image index is at the offset of the foreground image, start compositing images
            // This allows for a smaller foreground dimension
            if (c >= dx && c < scaledCols + dx && r >= dy && r < scaledRows + dy)
            {
                /// Blend each channel at i (the background), and j (foreground)
                bgImage[r * bgCols + c].r = blendColors(scaled[j].r, bgImage[(r)*bgCols + c].r, scaledMask[j].r);
                bgImage[r * bgCols + c].g = blendColors(scaled[j].g, bgImage[(r)*bgCols + c].g, scaledMask[j].g);
                bgImage[r * bgCols + c].b = blendColors(scaled[j].b, bgImage[(r)*bgCols + c].b, scaledMask[j].b);
                j++; // Increment the foreground
            }
    }

    /* write out the resulting image */
    writePPM(bgImage, bgRows, bgCols, bgColors /* s/b 255 */, argv[4]);

    /* free the image memory */
    free(scaled);
    free(scaledMask);
    free(bgImage);
    free(fgImage);
    free(mask);

    return (0);
}
