#include "Fractals.h"
#include <math.h>

void mandelbrot(Image *dst, float x0, float y0, float dx)
{
    float x, y, x1, y1, dy, zx, zy, sCols, sRows;
    int cols, rows, i, j, setX, setY;

    cols = dst->cols;
    rows = dst->rows;
    dy = dx * rows / cols;
    x1 = x0 + dx;
    y1 = y0 + dy;
    // calculate the number of columns cols = (x1 - x0) * rows / (y1 - y0)
    sCols = (x1 - x0) / cols;
    sRows = (y1 - y0) / rows;

    // allocate an image that is rows by cols -- done by the original
    // for each pixel in the image (i, j)
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            printf("Iterating through i, j: %d, %d\n", i, j);
            // calculate (x, y) given (i, j)
            // this corresponds to cx and cy in the Mandelbrot equation
            x = (sCols * j) + x0;
            y = (-1.0 * sRows * i) + y1;
            // set zx and zy to (0, 0)
            zx = zy = 0;
            // for some number of iterations up to N (e.g. 100)
            for (int n = 0; n < 1000; n++)
            {
                printf("Iterating the mandelbrot equation\n");
                // iterate the Mandelbrot equation
                x = pow(x, 2) - pow(y, 2) - sCols;
                y = 2 * x * y - sRows;
                printf("Calculated x to %.5f, y to %.5f\n", x, y);
                printf("n is %d\n", n);
                // if the length of z is greater than 2.0,
                if (sqrt(pow(x, 2) + pow(y, 2)) > 2.0)
                {
                    printf("Converting the numbers for pass\n");
                    // store the number of iterations and break
                    setX = i;
                    setY = j;
                    break;
                }
            }
            // color pixel (i, j)
            FPixel color = {{0, 0, 0}};
            image_setf(dst, setX, setY, color);
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
