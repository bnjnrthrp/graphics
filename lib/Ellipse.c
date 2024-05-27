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
    /*
    Parameters: cx, cy, Rx = maj ax, Ry = min ax
Initial value of x = -1
Initial value of y = -Ry
Initial value of px = 2 * Ry * Ry
Initial value of py = 2 * Rx * Rx * -y
Plot (x + cx, y + cy) and its four reflections
p = Ry * Ry - Rx * Rx * Ry + Rx*Rx/4 + Ry*Ry + px
*/
    int x, y, Rx, Ry, e, px, py, cx, cy;
    cx = e->c.val[0]; // center point x value
    cy = e->c.val[1]; // center point y value
    x = -1;
    y = -Ry;
    px = 2 * Ry * Ry;
    py = 2 * Rx * Rx * -y;
    e = Ry * Ry - Rx * Rx * Ry + Rx * Rx / 4 + Ry * Ry + px;
    // Plot the initial value and its 4 reflections
    image_setColor(src, y + cy, x + cx, p);
    image_setColor(src, y + cy, -x + cx - 1, p);
    image_setColor(src, -y + cy - 1, x + cx, p);
    image_setColor(src, -y + cy - 1, -x + cx - 1, p);

    while (px < py)
    {
        x--;
        px = px + 2 * Ry * Ry;
        if (e < 0)
        {
            e + Ry *Ry + px;
        }
        else
        {
            y++;
            py = py - 2 * Rx * Rx;
            e = e + Ry * Ry + px - py;
        }
        image_setColor(src, y + cy, x + cx, p);
        image_setColor(src, y + cy, -x + cx - 1, p);
        image_setColor(src, -y + cy - 1, x + cx, p);
        image_setColor(src, -y + cy - 1, -x + cx - 1, p);
    }

    e = Ry * Ry * (x * x + x) + Rx * Rx * (y * y - 2 * y + 1) - Rx * Rx * Ry * Ry + Rx * Rx - py;

    while (y < 0)
    {
        y++;
        py = py - 2 * Rx * Rx;
        if (e > 0)
        {
            e = e + Rx * Rx - py;
        }
        else
        {
            x--;
            px = px + 2 * Ry * Ry;
            e = e + Rx * Rx - py + px;
        }
        image_setColor(src, y + cy, x + cx, p);
        image_setColor(src, y + cy, -x + cx - 1, p);
        image_setColor(src, -y + cy - 1, x + cx, p);
        image_setColor(src, -y + cy - 1, -x + cx - 1, p);
    }
}
void ellipse_drawFill(Ellipse *e, Image *src, Color p);
