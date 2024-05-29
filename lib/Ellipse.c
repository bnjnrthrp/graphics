#include "Ellipse.h"

void ellipse_set(Ellipse *e, Point tc, double ta, double tb)
{
    e->ra = ta;
    e->rb = tb;
    e->c = tc;
    e->a = 0;
}
void ellipse_draw(Ellipse *e, Image *src, Color p)
{
    int quadrants[] = {1, 2, 3, 4};
    int size = 4;

    ellipse_draw_partial(e, src, p, quadrants, size);
}

void ellipse_draw_partial(Ellipse *e, Image *src, Color p, int *quadrants, int size)
{
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
    ellipse_draw_helper(src, y, cy, x, cx, p, quadrants, size);

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
        ellipse_draw_helper(src, y, cy, x, cx, p, quadrants, size);
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
        ellipse_draw_helper(src, y, cy, x, cx, p, quadrants, size);
    }
}

void ellipse_draw_helper(Image *src, int y, int cy, int x, int cx, Color p, int *quadrants, int size)
{

    if (size == 4)
    {
        image_setColor(src, y + cy, x + cx, p);           // Quadrant 2
        image_setColor(src, y + cy, -x + cx - 1, p);      // Quadrant 1
        image_setColor(src, -y + cy - 1, x + cx, p);      // Quadrant 3
        image_setColor(src, -y + cy - 1, -x + cx - 1, p); // Quadrant 4
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
void ellipse_drawFill(Ellipse *e, Image *src, Color p);
