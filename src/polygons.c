/**
 * This program draws the starfox ship using barycentric polygon triangles.
 * @author Benji Northrop
 */
#include <stdio.h>
#include <stdlib.h>
#include "Graphics.h"

static void draw_starfox(Image *src)
{
    if (!src)
        return;
    Point pt[100];
    Polygon *p;

    // Initialize all the colors. Each color is mapped to the corresponding index in the rgbValues arrays. Need to move this to hashmap
    Color colors[16];
    int colorSize = 16;
    float rValues[] = {0.63, 0.02, 0.51, 0.02, 0.08, 0.5, 0.39, 0.39, 0.46, 0.87, 0.27, 0.33, 0.74, 0.96, 1, 0.9};
    float gValues[] = {0.96, 0.29, 0.64, 0.3, 0.22, 0.6, 0.87, 0.77, 0.6, 0.93, 0.38, 0.42, 0.85, 0.81, 0.89, 1};
    float bValues[] = {0.97, 0.87, 1, 0.85, 0.3, 0.66, 0.99, 0.89, 1, 0.94, 0.45, 0.49, 0.92, 0.55, 0.79, 1};

    for (int i = 0; i < colorSize; i++)
    {
        color_set(&(colors[i]), rValues[i], gValues[i], bValues[i]);
    }

    // Define all the X and Y points
    int xValue[] = {825, 401, 875, 840, 724, 824, 793, 868, 895, 793, 895, 916, 805, 875, 788, 880, 792, 893, 968, 824, 1019, 875, 997, 1019, 997, 1002, 1030, 1019, 970, 836, 1012, 880, 887, 994, 1019, 999, 1024, 1135, 1060, 1196, 875, 1134, 401, 1135, 401, 1031, 873, 1196, 1060, 1469, 1186, 1236, 1260, 1812, 1233, 1271, 1260, 1493, 1523, 1557, 1812, 1064, 1168, 1050, 1174, 1050, 1180, 1076, 1080, 1159, 880, 1019, 1135, 1071};
    int yValue[] = {590, 901, 600, 623, 644, 590, 481, 550, 525, 481, 525, 230, 531, 543, 520, 538, 509, 528, 455, 590, 464, 600, 572, 464, 572, 557, 546, 464, 459, 585, 465, 594, 597, 566, 470, 553, 543, 618, 850, 552, 600, 619, 901, 635, 901, 674, 755, 552, 850, 304, 685, 620, 644, 626, 660, 675, 643, 652, 635, 643, 626, 579, 598, 570, 592, 555, 579, 526, 533, 575, 600, 463, 618, 486};
    int ptSize = 74;

    // Assign the points array
    for (int i = 0; i < ptSize; i++)
    {
        point_set2D(&(pt[i]), xValue[i], yValue[i]);
    }

    p = polygon_createp(3, pt);

    printf("Drawing the nose\n");
    polygon_drawFillB(p, src, colors[15]);

    polygon_set(p, 3, &(pt[1]));
    polygon_drawFillB(p, src, colors[15]);

    printf("Drawing left wing\n");
    polygon_set(p, 3, &(pt[4]));
    polygon_drawFillB(p, src, colors[1]);
    polygon_set(p, 3, &(pt[5]));
    polygon_drawFillB(p, src, colors[1]);

    polygon_set(p, 3, &(pt[6]));
    polygon_drawFillB(p, src, colors[6]);
    polygon_set(p, 3, &(pt[9]));
    polygon_drawFillB(p, src, colors[6]);

    printf("Drawing the left wing support\n");
    polygon_set(p, 3, &(pt[12]));
    polygon_drawFillB(p, src, colors[4]);
    polygon_set(p, 3, &(pt[13]));
    polygon_drawFillB(p, src, colors[4]);

    polygon_set(p, 3, &(pt[14]));
    polygon_drawFillB(p, src, colors[10]);
    polygon_set(p, 3, &(pt[15]));
    polygon_drawFillB(p, src, colors[10]);

    printf("Drawing Fuselage\n");
    polygon_set(p, 3, &(pt[70]));
    polygon_drawFillB(p, src, colors[5]);
    polygon_set(p, 3, &(pt[71]));
    polygon_drawFillB(p, src, colors[11]);

    printf("Drawing cockpit frame\n");
    polygon_set(p, 3, &(pt[18]));
    polygon_drawFillB(p, src, colors[11]);
    polygon_set(p, 3, &(pt[19]));
    polygon_drawFillB(p, src, colors[11]);

    polygon_set(p, 3, &(pt[20]));
    polygon_drawFillB(p, src, colors[4]);
    polygon_set(p, 3, &(pt[23]));
    polygon_drawFillB(p, src, colors[4]);
    polygon_set(p, 3, &(pt[24]));
    polygon_drawFillB(p, src, colors[4]);
    polygon_set(p, 3, &(pt[25]));
    polygon_drawFillB(p, src, colors[4]);

    printf("Drawing cockpit windows\n");
    polygon_set(p, 3, &(pt[28]));
    polygon_drawFillB(p, src, colors[7]);
    polygon_set(p, 3, &(pt[29]));
    polygon_drawFillB(p, src, colors[7]);

    printf("Drawing dark blue window\n");
    polygon_set(p, 3, &(pt[32]));
    polygon_drawFillB(p, src, colors[2]);
    polygon_set(p, 3, &(pt[33]));
    polygon_drawFillB(p, src, colors[2]);
    polygon_set(p, 3, &(pt[34]));
    polygon_drawFillB(p, src, colors[2]);

    printf("Drawing Right Wing\n");
    polygon_set(p, 3, &(pt[37]));
    polygon_drawFillB(p, src, colors[3]);

    printf("MidGray nose\n");
    polygon_set(p, 3, &(pt[40]));
    polygon_drawFillB(p, src, colors[12]);
    polygon_set(p, 3, &(pt[41]));
    polygon_drawFillB(p, src, colors[9]);

    polygon_set(p, 3, &(pt[44]));
    polygon_drawFillB(p, src, colors[12]);

    printf("Drawing Right Wing\n");
    polygon_set(p, 3, &(pt[47]));
    polygon_drawFillB(p, src, colors[8]);
    polygon_set(p, 3, &(pt[50]));
    polygon_drawFillB(p, src, colors[11]);
    polygon_set(p, 3, &(pt[51]));
    polygon_drawFillB(p, src, colors[11]);
    polygon_set(p, 3, &(pt[54]));
    polygon_drawFillB(p, src, colors[0]);

    polygon_set(p, 3, &(pt[55]));
    polygon_drawFillB(p, src, colors[5]);
    polygon_set(p, 3, &(pt[56]));
    polygon_drawFillB(p, src, colors[5]);
    polygon_set(p, 3, &(pt[57]));
    polygon_drawFillB(p, src, colors[13]);

    printf("Drawing right support\n");
    polygon_set(p, 3, &(pt[58]));
    polygon_drawFillB(p, src, colors[12]);
    polygon_set(p, 3, &(pt[61]));
    polygon_drawFillB(p, src, colors[4]);

    polygon_set(p, 3, &(pt[63]));
    polygon_drawFillB(p, src, colors[10]);
    polygon_set(p, 3, &(pt[64]));
    polygon_drawFillB(p, src, colors[10]);

    polygon_set(p, 3, &(pt[67]));
    polygon_drawFillB(p, src, colors[14]);

    image_write(src, "starfox.ppm");
    polygon_free(p);
}

/**
 * A program that will draw the pokemon Porygon
 */
static void draw_porygon(Image *src)
{
    if (!src)
        return;

    Point pt[100];
    Polygon *p;
    Circle circ;

    // Initialize all the colors. Each color is mapped to the corresponding index in the rgbValues arrays. Need to move this to hashmap
    Color colors[19];
    int colorSize = 19;
    float rValues[] = {0.81, 0.65, 1, 0.88, 0.91, 0.58, 0.65, 0.34, 0.26, 0.3, 0.32, 0.27, 0.35, 0.97, 0.94, 0.94, 0.89, 0.69, .05};
    float gValues[] = {0.91, 0.29, 0.35, 0.35, 0.53, 0.27, 0.84, 0.7, 0.45, 0.6, 0.67, 0.46, 0.71, 0.81, 0.65, 0.89, 0.85, 0.65, .05};
    float bValues[] = {0.91, 0.31, 0.4, 0.4, 0.55, 0.28, 0.89, 0.78, 0.49, 0.64, 0.72, 0.48, 0.79, 0.83, 0.69, 0.89, 0.85, 0.64, .05};

    for (int i = 0; i < colorSize; i++)
    {
        color_set(&(colors[i]), rValues[i], gValues[i], bValues[i]);
    }

    // Define all the X and Y points
    int xValue[] = {84, 118, 251, 162, 283, 306, 396, 513, 540, 453, 430, 345, 283, 252, 310, 330, 402, 236, 118, 236, 350, 451, 402, 453, 540, 330, 310, 402, 453, 345, 430, 245, 320, 274, 312, 305, 226, 274, 320, 508, 458, 449, 395, 274, 447, 559, 508, 545, 458, 650, 711, 508, 458, 449, 480, 650, 650, 626, 721, 738, 678, 711, 716, 731, 754, 508, 559, 600, 632, 711, 722, 734, 852, 774, 632, 585, 709, 728, 602, 852, 849, 728, 709, 774, 379};
    int yValue[] = {494, 348, 380, 522, 404, 471, 436, 314, 204, 241, 323, 338, 400, 379, 299, 209, 191, 156, 348, 156, 86, 103, 191, 241, 204, 209, 299, 191, 241, 338, 323, 497, 466, 590, 618, 667, 626, 590, 466, 634, 386, 384, 435, 590, 695, 732, 634, 491, 386, 337, 390, 634, 386, 384, 352, 337, 337, 335, 25, 29, 358, 390, 422, 419, 60, 634, 732, 670, 487, 390, 455, 456, 503, 559, 488, 762, 804, 837, 791, 503, 574, 837, 804, 559, 268};
    int ptSize = 85;

    // Assign the points array
    for (int i = 0; i < ptSize; i++)
    {
        point_set2D(&(pt[i]), xValue[i], yValue[i]);
    }

    p = polygon_createp(4, pt);

    printf("Drawing the nose\n");
    polygon_drawFill(p, src, colors[6]);
    polygon_set(p, 3, &(pt[2]));
    polygon_drawFill(p, src, colors[7]);
    polygon_set(p, 3, &(pt[3]));
    polygon_drawFill(p, src, colors[8]);

    printf("Drawing the cheeks\n");
    polygon_set(p, 8, &(pt[5]));
    polygon_drawFill(p, src, colors[1]);
    polygon_set(p, 4, &(pt[11]));
    polygon_drawFill(p, src, colors[2]);
    polygon_set(p, 6, &(pt[13]));
    polygon_drawFill(p, src, colors[13]);

    printf("Drawing the top of the head\n");
    polygon_set(p, 4, &(pt[19]));
    polygon_drawFill(p, src, colors[14]);
    polygon_set(p, 4, &(pt[21]));
    polygon_drawFill(p, src, colors[3]);

    printf("Drawing the eye\n");
    polygon_set(p, 3, &(pt[25]));
    polygon_drawFill(p, src, colors[15]);
    polygon_set(p, 4, &(pt[26]));
    polygon_drawFill(p, src, colors[16]);
    polygon_set(p, 3, &(pt[28]));
    polygon_drawFill(p, src, colors[17]);
    circle_set(&circ, pt[84], 15);
    circle_drawFill(&circ, src, colors[18]);

    printf("Drawing the left leg\n");
    polygon_set(p, 6, &(pt[31]));
    polygon_drawFill(p, src, colors[9]);

    printf("Drawing the chest\n");
    polygon_set(p, 3, &(pt[37]));
    polygon_drawFill(p, src, colors[0]);
    polygon_set(p, 5, &(pt[38]));
    polygon_drawFill(p, src, colors[10]);
    polygon_set(p, 4, &(pt[43]));
    polygon_drawFill(p, src, colors[11]);

    printf("Drawing the shoulder and body\n");
    polygon_set(p, 3, &(pt[46]));
    polygon_drawFill(p, src, colors[10]);
    polygon_set(p, 6, &(pt[46]));
    polygon_drawFill(p, src, colors[3]);
    polygon_set(p, 4, &(pt[52]));
    polygon_drawFill(p, src, colors[4]);

    printf("Drawing the tail\n");
    polygon_set(p, 5, &(pt[56]));
    polygon_drawFill(p, src, colors[0]);
    polygon_set(p, 6, &(pt[59]));
    polygon_drawFill(p, src, colors[11]);

    printf("Drawing the lower body\n");
    polygon_set(p, 4, &(pt[65]));
    polygon_drawFill(p, src, colors[5]);
    polygon_set(p, 3, &(pt[68]));
    polygon_drawFill(p, src, colors[5]);
    polygon_set(p, 4, &(pt[71]));
    polygon_drawFill(p, src, colors[12]);
    polygon_set(p, 4, &(pt[73]));
    polygon_drawFill(p, src, colors[0]);
    polygon_set(p, 4, &(pt[75]));
    polygon_drawFill(p, src, colors[11]);
    polygon_set(p, 5, &(pt[79]));
    polygon_drawFill(p, src, colors[12]);

    image_write(src, "porygon.ppm");
    polygon_free(p);
}

/*
Program to build out the starfox starship
*/
int main(int argc, char *argv[])
{
    Image *src;
    int rows = 1000;
    int cols = 2000;
    src = image_create(rows, cols);
    // For green screen
    image_fillrgb(src, 0, 1, 0);

    printf("Drawing Starfox's ship\n");
    draw_starfox(src);

    image_free(src);

    // rows = 900;
    // cols = 950;
    // src = image_create(rows, cols);

    // printf("Drawing Porygon\n");
    // draw_porygon(src);

    // image_free(src);

    return (0);
}
