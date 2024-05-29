#include "Circle.h"
#include <math.h>
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
 *
 */
void circle_draw_helper(Image *src, int y, int cy, int x, int cx, Color p, int *octants, int size)
{
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
    }
    else
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

void circle_draw_partial(Circle *c, Image *src, Color p, int *octants, int size)
{
    int x, y, e, cx, cy;
    // set x to -1
    x = -1;
    // set y to -radius (rounded)
    y = -floor(c->r);
    // set p to 1 - radius (rounded)
    e = 1 + y;

    cx = c->c.val[0];
    cy = c->c.val[1];

    circle_draw_helper(src, y, cy, x, cx, p, octants, size);

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
        circle_draw_helper(src, y, cy, x, cx, p, octants, size);
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
    int octants[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int size = 8;

    // Draws the circle with all 8 quadrants.
    circle_draw_partial(c, src, p, octants, size);
}

/**
 * Draw a filled circle into an image with a specified color
 * @param c the circle to draw
 * @param src the image to draw it into
 * @param p the color to draw
 */
void circle_drawFill(Circle *c, Image *src, Color p);
