#include "Ellipse.h"
#include "Line.h"

/**
 * Sets the values for an ellipse
 *
 * @param e the ellipse to set
 * @param tc the center point
 * @param ta the major axis radius
 * @param tb the minor axis radius
 */
void ellipse_set(Ellipse *e, Point tc, double ta, double tb)
{
    // Null check
    if (!e)
    {
        fprintf(stderr, "One of the pointers provided was null\n");
        exit(-1);
    }
    e->ra = ta;
    e->rb = tb;
    e->c = tc;
    e->a = 0;
}

/**
 * Bresenham's algorithm for drawing an ellipse
 *
 * @param e the ellipse
 * @param src the image to draw it n
 * @param p the color of the ellipse
 * @param quadrants the quadrants to draw (if partial ellipse)
 * @param size the number of quadrants to fill (used with quadrants)
 * @param fill 1 to fill the ellipse, 0 to not fill
 */
void ellipse_bresenham_algo(Ellipse *e, Image *src, Color p, int *quadrants, int size, int fill)
{
    // Null check
    if (!e || !src || !quadrants)
    {
        fprintf(stderr, "One of the pointers provided was null\n");
        exit(-1);
    }

    int x, y, Rx, Ry, err, px, py, cx, cy;
    cx = e->c.val[0]; // center point x value
    cy = e->c.val[1]; // center point y value
    Rx = e->ra;
    Ry = e->rb;
    x = -1;
    y = -Ry;
    px = 2 * Ry * Ry;
    py = 2 * Rx * Rx * -y;
    err = Ry * Ry - Rx * Rx * Ry + Rx * Rx / 4 + Ry * Ry + px;
    // Plot the initial value and its 4 reflections
    ellipse_draw_helper(src, y, cy, x, cx, p, quadrants, size, fill);

    while (px < py)
    {
        x--;
        px = px + 2 * Ry * Ry;
        if (err < 0)
        {
            err = err + Ry * Ry + px;
        }
        else
        {
            y++;
            py = py - 2 * Rx * Rx;
            err = err + Ry * Ry + px - py;
        }
        ellipse_draw_helper(src, y, cy, x, cx, p, quadrants, size, fill);
    }

    err = Ry * Ry * (x * x + x) + Rx * Rx * (y * y - 2 * y + 1) - Rx * Rx * Ry * Ry + Rx * Rx - py;

    while (y < 0)
    {
        y++;
        py = py - 2 * Rx * Rx;
        if (err > 0)
        {
            err = err + Rx * Rx - py;
        }
        else
        {
            x--;
            px = px + 2 * Ry * Ry;
            err = err + Rx * Rx - py + px;
        }
        ellipse_draw_helper(src, y, cy, x, cx, p, quadrants, size, fill);
    }
}

/**
 * Helper function to draw the ellipse. Can draw either a full ellipse or partial. Can also fill in a full ellipse
 *
 * @param src the image to draw to
 * @param y the y value of the pixel
 * @param cy the y value of the center of the ellipse
 * @param x the x value of the pixel
 * @param cx the x value of the center of the ellipse
 * @param p the color of the ellipse
 * @param quadrants array of which quadrants to fill in
 * @param size the number of quadrants to fill in
 * @param fill 1 to fill the ellipse, 0 to not fill
 */
void ellipse_draw_helper(Image *src, int y, int cy, int x, int cx, Color p, int *quadrants, int size, int fill)
{
    // Null check
    if (!src || !quadrants)
    {
        fprintf(stderr, "One of the pointers provided was null\n");
        exit(-1);
    }
    if (size == 4)
    {
        image_setColor(src, y + cy, x + cx, p);           // Quadrant 2
        image_setColor(src, y + cy, -x + cx - 1, p);      // Quadrant 1
        image_setColor(src, -y + cy - 1, x + cx, p);      // Quadrant 3
        image_setColor(src, -y + cy - 1, -x + cx - 1, p); // Quadrant 4
        if (fill)
        {
            Line fillRow;
            line_set2D(&fillRow, x + cx, y + cy + 1, -x + cx - 1, y + cy + 1);
            line_draw(&fillRow, src, p);

            line_set2D(&fillRow, x + cx, -y + cy, -x + cx - 1, -y + cy);
            line_draw(&fillRow, src, p);
        }
    }
    else
    {
        for (int i = 0; i < size; i++)
        {
            switch (quadrants[i])
            {
            case 1:
                image_setColor(src, y + cy, -x + cx - 1, p);
                break;
            case 2:
                image_setColor(src, y + cy, x + cx, p);
                break;
            case 3:
                image_setColor(src, -y + cy - 1, x + cx, p);
                break;
            case 4:
                image_setColor(src, -y + cy - 1, -x + cx - 1, p);
                break;
            default:
                break;
            }
        }
    }
}

/**
 * Draws a partial ellipse, based on which quadrants are requested
 *
 * @param e the ellipse to draw
 * @param src the image to draw it on
 * @param quadrants array of quadrants to draw
 */
void ellipse_draw_partial(Ellipse *e, Image *src, Color p, int *quadrants, int size)
{
    // Null check
    if (!e || !src || !quadrants)
    {
        fprintf(stderr, "One of the pointers provided was null\n");
        exit(-1);
    }
    ellipse_bresenham_algo(e, src, p, quadrants, size, 0);
}

/**
 * Draws a whole ellipse
 *
 * @param e the ellipse to draw
 * @param src the image to draw it on
 * @param p the color of the ellipse
 */
void ellipse_draw(Ellipse *e, Image *src, Color p)
{
    // Null check
    if (!e || !src)
    {
        fprintf(stderr, "One of the pointers provided was null\n");
        exit(-1);
    }

    int quadrants[] = {1, 2, 3, 4};
    int size = 4;

    ellipse_bresenham_algo(e, src, p, quadrants, size, 0);
}

/**
 * Draws and fills in an ellipse
 *
 * @param e the ellipse to draw/fill
 * @param image the image to draw it on
 * @param p the color of the ellipse
 */
void ellipse_drawFill(Ellipse *e, Image *src, Color p)
{
    // Null check
    if (!e || !src)
    {
        fprintf(stderr, "One of the pointers provided was null\n");
        exit(-1);
    }
    int quadrants[] = {1, 2, 3, 4};
    int size = 4;

    ellipse_bresenham_algo(e, src, p, quadrants, size, 1);
}
