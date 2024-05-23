/**
 * This document creates two types of fractals - Mandelbrot sets and Julia sets.
 *
 * Both use the same helper function, but can be generated independently.
 *
 * @author Benji Northrop
 */

#include "Fractals.h"

/**
 * Creates a mandelbrot or julia set given the origin and scale of x.
 *
 * @param dst the blank canvas
 * @param x0 the start location of the image on the x axis
 * @param y0 the start location of the image on the y axis
 * @param dx the scale from x0 to x1, where x1 is the end of the image
 * @param a the real component of c (used in julia set only)
 * @param bi the imaginary component of c (used in julia set only)
 * @param juliaSet True to generate a julia set, false to generate Mandelbrot set
 */
void mandelJuliaSet(Image *dst, float x0, float y0, float dx, float a, float bi, bool juliaSet)
{
    float cx, cy, x1, y1, dy, zx, zy, sCols, sRows, temp, r, g, b;
    int cols, rows, i, j, n, maxIterations;

    cols = dst->cols;
    rows = dst->rows;
    dy = dx * (float)rows / (float)cols;
    x1 = x0 + dx;
    y1 = y0 + dy;
    // calculate the number of columns cols = (x1 - x0) * rows / (y1 - y0)
    sCols = (x1 - x0) / (float)cols;
    sRows = (y1 - y0) / (float)rows;
    maxIterations = 200;

    // allocate an image that is rows by cols -- done by the original
    // for each pixel in the image (i, j)
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            // calculate (x, y) given (i, j)
            // this corresponds to cx and cy in the Mandelbrot equation
            if (juliaSet)
            {
                cx = a;
                cy = bi;
                zx = x0 + (sCols * j);
                zy = y1 - (sRows * i);
            }
            else
            {

                cx = x0 + (sCols * j);
                cy = y1 - (sRows * i);
                zx = zy = 0;
            }
            // set zx and zy to (0, 0)
            // for some number of iterations up to N (e.g. 100)
            n = 0;

            while (n < maxIterations && zx * zx + zy * zy <= 4)
            {

                // iterate the Mandelbrot equation
                temp = zx * zx - zy * zy - cx;
                zy = 2 * zx * zy - cy;
                zx = temp;

                n++;
            }
            // color pixel (i, j)
            if (n == maxIterations)
            {
                // Black inside
                r = g = b = 0;

                // Green screen inside
                // r = b = 0;
                // g = .75;
            }
            // else if (n > 30)
            // {
            //     // Green background
            //     r = 0;
            //     g = .75;
            //     b = 0;
            // }
            else
            {
                r = ((float)n / (float)maxIterations);
                g = ((float)n / (float)maxIterations);
                b = ((float)n / (float)maxIterations);
            }
            FPixel color = {{r, g, b}};
            image_setf(dst, i, j, color);
        }
    }
    // return the image
    return;
}

/**
 * Creates a mandelbrot set given the origin and scale of x.
 *
 * @param dst the blank canvas
 * @param x0 the start location of the image on the x axis
 * @param y0 the start location of the image on the y axis
 * @param dx the scale from x0 to x1, where x1 is the end of the image
 */
void mandelbrot(Image *dst, float x0, float y0, float dx)
{
    mandelJuliaSet(dst, x0, y0, dx, 0, 0, false);
}

/**
 * Creates a julia set given the origin and scale of x.
 *
 * @param dst the blank canvas
 * @param x0 the start location of the image on the x axis
 * @param y0 the start location of the image on the y axis
 * @param dx the scale from x0 to x1, where x1 is the end of the image
 */
void julia(Image *dst, float x0, float y0, float dx)
{
    float a, bi;
    a = 0.745405;
    bi = 0.1130063;
    mandelJuliaSet(dst, x0, y0, dx, a, bi, true);
}
