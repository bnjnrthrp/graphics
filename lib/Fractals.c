#include "Fractals.h"
#include <math.h>

void mandelbrot(Image *dst, float x0, float y0, float dx)
{
    float cx, cy, x1, y1, dy, zx, zx2, zy, zy2, sCols, sRows, temp;
    int cols, rows, i, j, n, maxIterations;
    unsigned char r, g, b;

    cols = dst->cols;
    rows = dst->rows;
    dy = dx * rows / cols;
    x1 = x0 + dx;
    y1 = y0 + dy;
    // calculate the number of columns cols = (x1 - x0) * rows / (y1 - y0)
    sCols = (x1 - x0) / cols;
    sRows = (y1 - y0) / rows;
    maxIterations = 100;

    // allocate an image that is rows by cols -- done by the original
    // for each pixel in the image (i, j)
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            // calculate (x, y) given (i, j)
            // this corresponds to cx and cy in the Mandelbrot equation
            cx = (sCols * j) + x0;
            cy = (sRows * i) + y1;
            // set zx and zy to (0, 0)
            zx = zy = 0;
            // for some number of iterations up to N (e.g. 100)
            n = 0;
            while (n < maxIterations)
            {

                // iterate the Mandelbrot equation
                zx2 = zx * zx - zy * zy - cx;
                zy2 = 2 * zx * zy - cy;

                zx = zx2;
                zy = zy2;
                // if the length of z is greater than 2.0,
                if (zx * zx + zy * zy > 4.0)
                {
                    // store the number of iterations and break
                    break;
                }
                n++;
            }
            // color pixel (i, j)
            if (n == maxIterations)
            {
                r = g = b = 0;
            }

            else
            {
                r = 255;
                g = 255;
                b = 255;
            }
            FPixel color = {{r, g, b}};
            image_setf(dst, i, j, color);
        }
    }
    // return the image
    return;
}
void julia(Image *dst, float x0, float y0, float dx) { return; }

/**
 *
 * A picture of the complete Mandelbrot set in an appropriate rectangle.
A picture of a Julia set defined by c = 0.7454054 + i*0.1130063 in an appropriate rectangle.
*/
