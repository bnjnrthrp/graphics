#include "Circle.h"
#include "Line.h"
#include <math.h>
#include <stdlib.h>
/**
 * Initialize a circle to center tc and radius tr.
 * @param c the Circle pointer
 * @param tc the center point
 * @param tr the radius
 */
void circle_set(Circle *c, Point tc, double tr)
{
    // Null check
    if (c)
    {
        c->c = tc;
        c->r = tr;
    }
}

/**
 * Helper function that will draw either the full circle or a partial circle
 *
 * @param src the image to draw to
 * @param y the y value
 * @param cy the y value of the center of the circle
 * @param x the x value
 * @param cx the x value of the center of the circle
 * @param p the color to draw
 * @param octants an array that contains the desired octants to draw
 * @param size the size of the octants array (how many octants to fill)
 * @param fill 1 to fill the circle, 0 to not fill
 */
void circle_draw_helper(Image *src, int y, int cy, int x, int cx, Color p, int *octants, int size, int fill)
{
    // Null check
    if (!src || !octants)
    {
        fprintf(stderr, "One of the pointers provided was null\n");
        exit(-1);
    }
    // If drawing the whole circle, shortcut
    if (size == 8)
    {
        image_setColor(src, y + cy, x + cx, p);           // Octant 3
        image_setColor(src, y + cy, -x + cx - 1, p);      // Octant 2
        image_setColor(src, x + cy, y + cx, p);           // Octant 4
        image_setColor(src, x + cy, -y + cx - 1, p);      // Octant 1
        image_setColor(src, -y + cy - 1, x + cx, p);      // Octant 6
        image_setColor(src, -y + cy - 1, -x + cx - 1, p); // Octant 7
        image_setColor(src, -x + cy - 1, y + cx, p);      // Octant 5
        image_setColor(src, -x + cy - 1, -y + cx - 1, p); // Octant 8
        if (fill)
        {
            Line fillRow;
            line_set2D(&fillRow, x + cx, y + cy + 1, -x + cx - 1, y + cy + 1); // Octants 3 and 2
            line_draw(&fillRow, src, p);

            line_set2D(&fillRow, y + cx, x + cy + 1, -y + cx - 1, x + cy + 1); // Octants 4 and 1
            line_draw(&fillRow, src, p);

            line_set2D(&fillRow, x + cx, -y + cy, -x + cx - 1, -y + cy); // Octants 6 and 7
            line_draw(&fillRow, src, p);

            line_set2D(&fillRow, y + cx, -x + cy, -y + cx - 1, -x + cy); // Octants 5 and 8
            line_draw(&fillRow, src, p);
        }
    }
    else // Otherwise determine which quadrants to fill
    {
        for (int i = 0; i < size; i++)
        {
            switch (octants[i])
            {
            case 1:
                image_setColor(src, x + cy, -y + cx - 1, p); // Octant 1
                break;
            case 2:
                image_setColor(src, y + cy, -x + cx - 1, p); // Octant 2
                break;
            case 3:
                image_setColor(src, y + cy, x + cx, p); // Octant 3
                break;
            case 4:
                image_setColor(src, x + cy, y + cx, p); // Octant 4
                break;
            case 5:
                image_setColor(src, -x + cy - 1, y + cx, p); // Octant 5
                break;
            case 6:
                image_setColor(src, -y + cy - 1, x + cx, p); // Octant 6
                break;
            case 7:
                image_setColor(src, -y + cy - 1, -x + cx - 1, p); // Octant 7
                break;
            case 8:
                image_setColor(src, -x + cy - 1, -y + cx - 1, p); // Octant 8
            default:
                break;
            }
        }
    }
}

/**
 * Bresenham's algorithm to draw a circle
 *
 * @param c the circle to draw
 * @param src the Image to draw it to
 * @param p the color of the circle
 * @param octants the octants to draw (for partial drawings)
 * @param size the size of the array of octants
 * @param fill 1 to fill the circle, 0 to not fill the circle
 */
void circle_bresenham_algo(Circle *c, Image *src, Color p, int *octants, int size, int fill)
{
    // Null check
    if (!c || !src || !octants)
    {
        fprintf(stderr, "One of the pointers provided was null\n");
        exit(-1);
    }
    int x, y, e, cx, cy;
    // set x to -1
    x = -1;
    // set y to -radius (rounded)
    y = -floor(c->r);
    // set p to 1 - radius (rounded)
    e = 1 + y;

    cx = c->c.val[0];
    cy = c->c.val[1];

    circle_draw_helper(src, y, cy, x, cx, p, octants, size, fill);

    while (x > y)
    {
        x--;
        if (e < 0)
        {
            e = e + 1 - 2 * x;
        }
        else
        {
            y++;
            e = e + 1 - 2 * (x - y);
        }
        circle_draw_helper(src, y, cy, x, cx, p, octants, size, fill);
    }
}

/**
 * Draw the circle into the image with a specified color
 * @param c the circle to draw
 * @param src the image to draw it into
 * @param p the color to draw
 */
void circle_draw(Circle *c, Image *src, Color p)
{
    // Null check
    if (!c || !src)
    {
        fprintf(stderr, "One of the pointers provided was null\n");
        exit(-1);
    }

    int octants[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int size = 8;

    // Draws the circle with all 8 quadrants.
    circle_bresenham_algo(c, src, p, octants, size, 0);
}

/**
 * Draws a partial circle by octant
 * @param c the circle to draw
 * @param src the image to draw onto
 * @param p the color to draw
 * @param octants the octants to draw
 * @param size the size of the array of octants to draw
 */
void circle_draw_partial(Circle *c, Image *src, Color p, int *octants, int size)
{
    // Null check
    if (!c || !src || !octants)
    {
        fprintf(stderr, "One of the pointers provided was null\n");
        exit(-1);
    }
    // Call the helper function and do not fill
    circle_bresenham_algo(c, src, p, octants, size, 0);
}

/**
 * Draw a filled circle into an image with a specified color
 * @param c the circle to draw
 * @param src the image to draw it into
 * @param p the color to draw
 */
void circle_drawFill(Circle *c, Image *src, Color p)
{
    // Null check
    if (!c || !src)
    {
        fprintf(stderr, "One of the pointers provided was null\n");
        exit(-1);
    }
    int octants[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int size = 8;

    // Draws the circle with all 8 quadrants.
    circle_bresenham_algo(c, src, p, octants, size, 1);
}
