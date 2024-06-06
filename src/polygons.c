/**
 * This program draws the starfox ship using barycentric polygon triangles.
 * @author Benji Northrop
 */
#include <stdio.h>
#include <stdlib.h>
#include "Graphics.h"

/*
Program to build out the starfox starship
*/

int main(int argc, char *argv[])
{
    Point pt[100];
    Polygon *p;
    Image *src;
    int rows = 1000;
    int cols = 2000;

    // Initialize all the colors. Each color is mapped to the corresponding index in the rgbValues arrays. Need to move this to hashmap
    Color colors[16];
    int colorSize = 16;
    float rValues[] = {0.63, 0.27, 0.51, 0.02, 0.08, 0.5, 0.39, 0.39, 0.46, 0.87, 0.27, 0.33, 0.74, 0.96, 1, 0.9};
    float gValues[] = {0.96, 0.84, 0.64, 0.3, 0.22, 0.6, 0.87, 0.77, 0.6, 0.93, 0.38, 0.42, 0.85, 0.81, 0.89, 1};
    float bValues[] = {0.97, 1, 1, 0.85, 0.3, 0.66, 0.99, 0.89, 1, 0.94, 0.45, 0.49, 0.92, 0.55, 0.79, 1};

    for (int i = 0; i < colorSize; i++)
    {
        color_set(&(colors[i]), rValues[i], gValues[i], bValues[i]);
    }

    // Define all the X and Y points
    int xValue[] = {825, 401, 875, 840, 724, 824, 793, 868, 895, 793, 895, 916, 805, 875, 788, 880, 792, 893, 968, 824, 1019, 875, 997, 1019, 997, 1002, 1030, 1019, 970, 836, 1012, 880, 887, 994, 1019, 999, 1024, 1135, 1060, 1196, 875, 1134, 401, 1135, 401, 1031, 873, 1196, 1060, 1469, 1186, 1236, 1260, 1812, 1233, 1271, 1260, 1493, 1523, 1557, 1812, 1064, 1168, 1050, 1174, 1050, 1180, 1076, 1080, 1159};
    int yValue[] = {590, 901, 600, 623, 644, 590, 481, 550, 525, 481, 525, 230, 531, 543, 520, 538, 509, 528, 455, 590, 464, 600, 572, 464, 572, 557, 546, 464, 459, 585, 465, 594, 597, 566, 470, 553, 543, 618, 850, 552, 600, 619, 901, 635, 901, 674, 755, 552, 850, 304, 685, 620, 644, 626, 660, 675, 643, 652, 635, 643, 626, 579, 598, 570, 592, 555, 579, 526, 533, 575};
    int ptSize = 69;

    for (int i = 0; i < ptSize; i++)
    {
        point_set2D(&(pt[i]), xValue[i], yValue[i]);
        printf("Inserted point\n");
        point_print(&(pt[i]), stdout);
    }
    // Initialize the points array

    src = image_create(rows, cols);

    image_write(src, "starfox.ppm");

    image_free(src);

    return (0);
}
