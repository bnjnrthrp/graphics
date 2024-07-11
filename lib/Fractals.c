/**
 * This document creates two types of fractals - Mandelbrot sets and Julia sets.
 *
 * Both use the same helper function, but can be generated independently.
 *
 * @author Benji Northrop
 */

#include "Fractals.h"

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

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
                /// Color with sin/cos in red/green channels
                r = ((float)sin(n) / (float)maxIterations);
                g = ((float)cos(n) / (float)maxIterations);
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

/**
 * Builds a heightmap on a 1 x 1 grid that will be subdivided a given number of times. Adds a perturbation to each midpoint as it subdivides
 *
 * @param md the module to add the terrain submap to
 * @param prevMap The 2D matrix with the previous iteration's height values
 * @param count the current iteration number
 * @param maxIterations the total number of iterations desired
 * @param roughness the roughness factor for calculating the size of the perturbations
 */
void buildHeightMap(Module *md, int oldRows, int oldCols, double prevMap[oldRows][oldCols], int count, int maxIterations, double roughness)
{
    int i, j, newRows, newCols;
    double x1, x2, y, z1, z2, prev, next, length, avgHeight;
    Point pt[4];
    Polygon p;
    Color White, Green, Blue, Brown;

    color_set(&White, 1.0, 1.0, 1.0);
    color_set(&Blue, 0.0, 0.0, 0.7);
    color_set(&Green, 0.15, .5, 0.15);
    color_set(&Brown, 0.55, 0.35, 0.0);

    if (!md || !prevMap || maxIterations < 0)
    {
        fprintf(stderr, "Invalid pointer provided to buildHeightMap\n");
        exit(-1);
    }

    // Base case: if iterations == count, then we're ready to draw
    if (count == maxIterations)
    {
        // Initialize the polygon for drawing
        polygon_init(&p);

        // Iterate through the matrix and create the points
        for (i = 0; i < oldRows - 1; i++)
        {
            for (j = 0; j < oldCols - 1; j++)
            {
                // x value becomes the index / cols and z value is z / rows. Builds a triangle mesh 2 triangles at a time
                // y Value is determined by the matrix at that position
                x1 = (double)j / (double)oldCols;
                x2 = (double)(j + 1) / (double)oldCols;
                z1 = (double)i / (double)oldRows;
                z2 = (double)(i + 1) / (double)oldRows;
                point_set3D(&pt[0], x1, prevMap[i][j], z1);
                point_set3D(&pt[1], x2, prevMap[i][j + 1], z1);
                point_set3D(&pt[2], x1, prevMap[i + 1][j], z2);
                point_set3D(&pt[3], x2, prevMap[i + 1][j + 1], z2);

                avgHeight = (pt[0].val[1] + pt[1].val[1] + pt[2].val[1] + pt[3].val[1]) / 4.0;

                if (avgHeight > .4)
                {
                    module_color(md, &White);
                }
                else if (avgHeight > 0.2)
                {
                    module_color(md, &Brown);
                }
                else if (avgHeight > 0.0)
                {
                    module_color(md, &Green);
                }
                else
                {
                    module_color(md, &Blue);
                }
                // Add triangles to the module
                polygon_set(&p, 3, &pt[0]);
                module_polygon(md, &p);

                polygon_set(&p, 3, &pt[1]);
                module_polygon(md, &p);
            }
        }
        // Cleanup
        polygon_clear(&p);
    }
    else
    {
        count++; // Increase the counter for this cycle
        // Calculate the size of the expanded height map
        newRows = newCols = pow(2, count) + 1;
        // Initialize a new height map with the expanded size
        double newMap[newRows][newCols];
        for (i = 0; i < newRows; i++)
        {
            for (j = 0; j < newCols; j++)
            {
                newMap[i][j] = 0;
            }
        }

        // Copy the values from the old heightmap to the new one
        for (i = 0; i < oldRows; i++)
        {
            for (j = 0; j < oldCols; j++)
            {
                newMap[2 * i][2 * j] = prevMap[i][j];
            }
        }

        // Loop through new map now, and interpolate the height values that remain in each row first
        /**
         *  **Note - the values changing from 0 to something will have a random perturbation factor added in
         *  old map     new map         first pass      second pass
         *  1 3 1       1 0 3 0 1       1 2 3 2 1       1 2 3 2 1
         *  3 5 3   ->  0 0 0 0 0   ->  0 0 0 0 0  ->   2 3 4 3 2
         *  1 3 1       3 0 5 0 3       3 4 5 4 3       3 4 5 4 3
         *              0 0 0 0 0       0 0 0 0 0       2 3 4 3 2
         *              1 0 3 0 1       1 2 3 2 1       1 2 3 2 1
         */
        for (i = 0; i < newRows; i = i + 2)
        {
            for (j = 0; j < newCols; j++)
            {
                if (newMap[i][j] == 0)
                {
                    prev = newMap[i][j - 1];
                    next = newMap[i][j + 1];

                    // Perturb the value between these two and then insert the data
                    y = average(prev, next);
                    y = perturb(y, roughness, count);

                    newMap[i][j] = y;
                }
            }
        }

        // Repeat loop in the remaining rows to fill the zeroes, interpolate from the
        // upper and lower values and perturb
        for (i = 1; i < newRows - 1; i = i + 2)
        {
            for (j = 0; j < newCols; j++)
            {
                // Perturb the value between these two and then insert the data
                if (newMap[i][j] == 0)
                {
                    prev = newMap[i - 1][j];
                    next = newMap[i + 1][j];

                    // Perturb the value between these two and then insert the data
                    y = average(prev, next);

                    y = perturb(y, roughness, count);
                    newMap[i][j] = y;
                }
            }
        }
        // Recursively call the heightmap again, with the new count and map
        buildHeightMap(md, newRows, newCols, newMap, count, maxIterations, roughness);
    }
}

/**
 * Calculates the average between two values
 */
double average(double a, double b)
{
    return (a + b) / 2.0;
}

/**
 * Applies a perturbation to a point, given a roughness scale and length. Adjusts the y coordinate by adding this perturbation.
 * Perturbation = random * 2.0 - 1, giving us a a range between -1.0 to 1.0. This is then multiplified by length ^ roughness to
 * provide a final adjustment range of (-l^r, l^r). This allows for smaller adjustments when adjusting the midpoint of two points close together.
 *
 * @param y the midpoint to adjust
 * @param r the roughness value, needs to be something <= 1
 * @param i the iteration count
 *
 * @return the adjusted y value
 */
double perturb(double y, double r, double i)
{
    // Calculate a random number between -1.0 and 1.0
    double adjustment = ((drand48() * 2.0) - 1.0);

    // Adjust y by the perturbation amount
    y = y + adjustment * pow(r, i);

    return y;
}

/**
 * Determines the distance between 2 points
 *
 * @param a the source point
 * @param b the destination point
 * @return the distance from a to b
 */
double getLength(double x1, double y1, double z1, double x2, double y2, double z2)
{
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
}
