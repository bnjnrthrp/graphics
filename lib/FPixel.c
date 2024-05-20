#include "FPixel.h"
#include "ppmIO.h"
#include "Color.h"
#include <stdio.h>

/**
 * Converts a Pixel array of unsigned chars into an FPixel array of float
 *
 * @param pixel the source pixel array
 * @param rows the number of rows in the image
 * @param cols the number of columns in the image
 * @param dest the destination FPixel array
 */
void convert_Pixel(Pixel *pixel, int rows, int cols, FPixel **dest)
{
    for (int i = 0; i < rows * cols; i++)
    {

        pixel[i].r = uc_to_float(dest[0][i].rgb[0]);
        pixel[i].g = uc_to_float(dest[0][i].rgb[1]);
        pixel[i].b = uc_to_float(dest[0][i].rgb[2]);
    }
}

/**
 * Converts an FPixel array of floats to a Pixel array of unsigned chars
 *
 * @param fpixel the source FPixel array
 * @param rows the number of rows in the image
 * @param cols the number of columns in the image
 * @param dest the destination Pixel array
 */
void convert_FPixel(FPixel **fpixel, int rows, int cols, Pixel *dest)
{
    for (int i = 0; i < rows * cols; i++)
    {
        dest[i].r = float_to_uc(fpixel[0][i].rgb[0]);
        dest[i].g = float_to_uc(fpixel[0][i].rgb[1]);
        dest[i].b = float_to_uc(fpixel[0][i].rgb[2]);
    }
};
