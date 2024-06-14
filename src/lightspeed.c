/*
An example of building a background for light speed travel*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/Graphics.h"
#define M_PI 3.14159265358979323846

int main(int argc, char *argv[])
{
    const int nLines = 200;
    const int nFrames = 25;
    const int rows = 1000;
    const int cols = 2000;
    View2D view;
    Matrix vtm;
    Matrix ltm;
    Point point[nLines];
    Point point2[nLines];
    Line line[nLines];
    Line tline;
    Color White, Black;
    int i, t;
    char filename[256];
    Image *src;

    color_set(&White, 1, 1, 1);
    color_set(&Black, 0, 0, 0);

    src = image_create(rows, cols);

    srand48(56);

    // Make a bunch of random points in the [0, 1] square
    for (i = 0; i < nLines; i++)
    {
        point_set2D(&(point[i]), drand48(), drand48());
    }

    // Set up a view on (1.5, 1.5)
    point_set2D(&(view.vrp), 1, 0);
    view.dx = 1.0;
    vector_set(&(view.x), 1.0, 0.0, 0.0);
    view.screenx = cols;
    view.screeny = rows;

    matrix_setView2D(&vtm, &view);
    printf("vtm:\n");
    matrix_print(&vtm, stdout);

    // Loop for some number of frames
    for (t = 0; t < nFrames; t++)
    {
        double cubic = (double)(t * t * t) / (double)(nFrames * nFrames * nFrames);
        image_fillc(src, Black);
        for (i = 0; i < nLines; i++)
        {
            // Adjust the second point as a cubic function. This results in a slow start but fast acceleration
            point_set2D(&(point2[i]),
                        point[i].val[0] + cubic,
                        point[i].val[1]);
            line_set(&(line[i]), point[i], point2[i]);
            tline = line[i];

            // translate the line as we move
            matrix_identity(&ltm);
            // Rotate the line up ~45 degrees to align with ship
            matrix_translate2D(&ltm, -tline.a.val[0], -tline.a.val[1]);
            matrix_rotateZ(&ltm, cos(M_PI / 4.0), sin(-M_PI / 4.0));
            matrix_translate2D(&ltm, tline.a.val[0], tline.a.val[1]);

            // Translate the lines towards the upper right, slowly at first
            matrix_translate2D(&ltm, cubic, -cubic);
            // Scale the lines to the size of the image
            matrix_scale2D(&ltm, view.screenx, view.screeny);
            matrix_xformLine(&ltm, &tline);

            // Draw the line
            line_draw(&tline, src, White);
        }
        // printf("Writing file\n");
        sprintf(filename, "lightspeed-%03d.ppm", t);
        image_write(src, filename);
    }

    image_free(src);

    return (0);
}
