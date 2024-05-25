#include <Math.h>
#include "Line.h"

void line_set2D(Line *l, double x0, double y0, double x1, double y1)
{
    Point a, b;
    a = l->a;
    b = l->b;
    point_set2D(&a, x0, y0);
    point_set2D(&b, x1, y1);
    l->zBuffer = 1;
}
void line_set(Line *l, Point ta, Point tb)
{
    l->a = ta;
    l->b = tb;
    l->zBuffer = 1;
}
void line_zBuffer(Line *l, int flag)
{
    if (flag == 0 || flag == 1)
        l->zBuffer = flag;
}
void line_normalize(Line *l)
{
    // Null check
    if (l)
    {
        point_normalize(&(l->a));
        point_normalize(&(l->b));
    }
}
void line_copy(Line *to, Line *from)
{
    // Null check
    if (to && from)
    {
        to->a = from->a;
        to->b = from->b;
        to->zBuffer = from->zBuffer;
    }
}

void line_draw(Line *l, Image *src, Color c)
{
    int x, y, dx, dy, e, stepX, stepY;
    double x0, x1, y0, y1;
    // Null check
    if (l && src)
    {
        // Uses modified Bresenham's algorithm
        x0 = l->a.val[0];
        y0 = l->a.val[1];
        x1 = l->b.val[0];
        y1 = l->b.val[1];
        dx = x1 - x0;
        dy = y1 - y0;

        /**
         * Determine the octant:
         * 1st- step by x to right, test y
         * 2nd- step by y, test x
         * 3rd- step by y, test x (x steps left)
         * 4th- step by x (to the left), test y
         * 5 mirrors 4
         * 6 mirrors 3
         * 7 mirrors 2
         * 8 mirrors 1
         *
         * Horizontal and Vertical special cases:
         * Hor left to right -> 1st quadrant
         * Vertical down to up -> 2nd quadrant
         * Horizontal right to left -> 3rd quadrant
         * Vertical up to down -> 4th quadrant
         * In the point of view of the line, draw to the left of the line.
         */
        // Determine the octant
        // Octant 1, step right, positive slope, dx > dy
        if (abs(dx) > abs(dy)) // Octant, 1, 4, 5, or 8
        {
            // Octant 1 or 8
            if (dx > 0)
            {
                // X steps to the right
                stepX = 1;
                if (dy > 0) // Octant 1
                {
                    stepY = 1;
                }
                else // Octant 8
                {
                    dy = -dy; // Set dy to its positive value so the algorithm will work.
                    stepY = -1;
                }
            }
            // Octant 4 or 5
            else
            {
                dx = -dx;
                // X steps to the left
                stepX = -1;
                if (dy > 0) // Octant 4
                {
                    stepY = 1;
                }
                else // Octant 5
                {
                    dy = -dy;
                    stepY = -1;
                }
            }
            // Step through the Bresenham algorithm for (int i = 0; i < dx; i++)
            {
                // Set the initial error
                e = 3 * dy - 2 * dx;
                // Set the color of the current pixel
                image_setColor(src, y, x, c);
                // While the error is positive, we need to step up
                while (e > 0)
                {
                    // Step one y pixel
                    y += stepY;
                    // Subtract 1 from e to account for the step forward.
                    // Becomes 2*dx for the integer math
                    e = e - 2 * dx;
                }
                // Step one x pixel
                x = stepX;
                // Step forward with dy (3 dy to account for new slope of 3/2)
                e = e + 2 * dy;
            }
        }
        else
        {
            // More rise than run, step through y as the primary.
            if (abs(dx) < abs(dy)) // Octant, 2, 3, 6, or 7
            {
                // Octant 2 or 7
                if (dx > 0)
                {
                    // X steps to the right
                    stepX = 1;
                    if (dy > 0) // Octant 2
                    {
                        stepY = 1;
                    }
                    else // Octant 7
                    {
                        dy = -dy;
                        stepY = -1;
                    }
                }
                // Octant 3 or 6
                else
                {
                    dx = -dx;
                    // X steps to the left
                    stepX = -1;
                    if (dy > 0) // Octant 3
                    {
                        stepY = 1;
                    }
                    else // Octant 6
                    {
                        dy = -dy;
                        stepY = -1;
                    }
                }
                // Step through the Bresenham algorithm for (int i = 0; i < dx; i++)
                {
                    // Set the initial error
                    e = 3 * dy - 2 * dx;
                    // Set the color of the current pixel
                    image_setColor(src, y, x, c);
                    // While the error is positive, we need to step up
                    while (e > 0)
                    {
                        // Step one x pixel
                        x += stepX;
                        // Subtract 1 from e to account for the step left or right.
                        // Becomes 2*dy for the integer math
                        e = e - 2 * dy;
                    }
                    // Step one y pixel
                    y = stepY;
                    // Step forward with dx
                    e = e + 2 * dx;
                }
            }
        }
    }
}
