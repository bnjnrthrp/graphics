#include <stdlib.h>
#include <math.h>
#include "Line.h"
#include "Point.h"

/**
 * Sets the two points of a line given the x, y values
 *
 * @param l the line to set
 * @param x0 point 1 x value
 * @param y0 point 1 y value
 * @param x1 point 2 x value
 * @param y1 point 2 y value
 */
void line_set2D(Line *l, double x0, double y0, double x1, double y1)
{
    // Null check
    if (!l)
    {
        fprintf(stderr, "A null pointer was provided\n");
        exit(-1);
    }
    Point a, b;
    point_set2D(&a, x0, y0);
    point_set2D(&b, x1, y1);
    l->a = a;
    l->b = b;
    l->zBuffer = 1;
}

/**
 * Sets the line data based on 2 points
 * @param l the line to modify
 * @param ta point 1
 * @param tb point 2
 */
void line_set(Line *l, Point ta, Point tb)
{
    // Null check
    if (!l)
    {
        fprintf(stderr, "A null pointer was provided\n");
        exit(-1);
    }
    l->a = ta;
    l->b = tb;
    l->zBuffer = 1;
}

/**
 * Sets the zBuffer of a line
 * @param l the line to modify
 * @param flag 1 for true, 0 for false
 */
void line_zBuffer(Line *l, int flag)
{
    // Null check
    if (!l)
    {
        fprintf(stderr, "A null pointer was provided\n");
        exit(-1);
    }
    if (flag == 0 || flag == 1)
        l->zBuffer = flag;
}

/**
 * Normalizes the x, y values of a line
 * @param l the line to normalize
 */
void line_normalize(Line *l)
{
    // Null check
    if (!l)
    {
        fprintf(stderr, "A null pointer was provided\n");
        exit(-1);
    }
    point_normalize(&(l->a));
    point_normalize(&(l->b));
}
/**
 * Copies the line info from one line to another
 * @param to the destination line
 * @param from the source line
 */
void line_copy(Line *to, Line *from)
{
    // Null check
    if (!to || !from)
    {
        fprintf(stderr, "A null pointer was provided\n");
        exit(-1);
    }

    to->a = from->a;
    to->b = from->b;
    to->zBuffer = from->zBuffer;
}

/**
 * Draws a line using Bresenham's algorithm
 * @param l the line to draw
 * @param src the image to draw it on
 * @param c the color of the line
 */
void line_draw(Line *l, Image *src, Color c)
{
    int x, y, dx, dy, e, stepX, stepY;
    double x0, x1, y0, y1;

    // Null check
    if (!l || !src)
    {
        fprintf(stderr, "A null pointer was provided\n");
        exit(-1);
    }

    // Uses modified Bresenham's algorithm
    x0 = l->a.val[0];
    y0 = l->a.val[1];
    x1 = l->b.val[0];
    y1 = l->b.val[1];
    dx = x1 - x0;
    dy = y1 - y0;
    x = x0;
    y = y0;

    /**
     * Special cases: determine if horizontal or vertical line. Horizontal will draw left to right on top, right to left on pixel row beneath it
     *
     * Vertical lines will draw to the left when drawing down to up, and to the right when drawing up to down.
     */
    if (dy == 0)
    { // Horizontal Line
        printf("horizontal line\n");
        stepX = 1;  // Set the step to go to the right.
        if (dx < 0) // if dx < 0, line goes left, so step left and draw in 3rd quadrant
        {
            // Swap order of so dx is positive
            dx = -dx;
            stepX = -1;
            // Reverse the order of x so it draws left to right
            x = x - 1; // Start one pixel left
        }
        else // Line goes right, draw in 1st quadrant.
        {
            y = y0 - 1; // Sets the row to be one beneath the original ask - so both draw on top
        }
        for (int i = 0; i < dx; i++) // -1 to drop the final pixel
        {
            image_setColor(src, y, x, c);
            x += stepX;
        }
    }
    else if (dx == 0)
    { // Vertical Line
        printf("Vertical line\n");
        stepY = -1; // Set the step to go up (0, 0) in top left corner
        if (dy > 0) // Positive dy means drawing down, so draw in 4th quadrant
        {
            stepY = 1; // Set step to step down
        }
        else // Draw line up (2nd quadrant)
        {
            dy = -dy;   // flip dy to make it positive for the count
            y = y - 1;  // Start one higher than y0
            x = x0 - 1; // Sets the col to be one left of the original - so we draw to the "left" of the pixel line.
        }
        for (int i = 0; i < dy; i++)
        {
            image_setColor(src, y, x, c);
            y += stepY;
        }
    }
    else
    {
        /**
         * Determine the octant:
         * 1st- step by x to right, test y:         |dx| > |dy|, dx > 0 & dy < 0
         * 2nd- step by y, test x                   |dx| < |dy|, dx > 0 & dy < 0
         * 3rd- step by y, test x (x steps left)    |dx| < |dy|, dx < 0 & dy < 0
         * 4th- step by x (to the left), test y     |dx| > |dy|, dx < 0 & dy < 0
         *
         */
        // Determine the octant
        // Octant 1, step right, positive slope, dx > dy
        if (abs(dx) >= abs(dy)) // Octant, 1, 4, 5, or 8
        {
            // Octant 1 or 8
            if (dx > 0)
            {
                // X steps to the right
                stepX = 1;
                if (dy < 0) // Octant 1
                {
                    printf("Octant 1\n");
                    dy = -dy;  // Make dy positive so the algorithm will work
                    y = y - 1; // Start one higher than y0
                    stepY = -1;
                }
                else // Octant 8
                {
                    printf("Octant 8\n");
                    stepY = 1;
                }
            }
            // Octant 4 or 5
            else
            {
                x = x - 1;
                dx = -dx;
                // X steps to the left
                stepX = -1;
                if (dy < 0) // Octant 4
                {
                    printf("Octant 4\n");
                    y = y - 1;
                    dy = -dy;
                    stepY = -1;
                }
                else // Octant 5
                {
                    printf("Octant 5\n");
                    stepY = 1;
                }
            }
            // Step through the Bresenham algorithm for (int i = 0; i < dx; i++)
            {
                // Set the initial error
                e = 3 * dy - 2 * dx;

                // Step through all the x's
                for (int i = 0; i < dx; i++)
                {
                    // Set the color of the current pixel
                    image_setColor(src, y, x, c);

                    // if the error is positive, we need to step up
                    if (e > 0)
                    {
                        // Step one y pixel
                        y += stepY;
                        // Subtract 1 from e to account for the step forward.
                        // Becomes 2*dx for the integer math
                        e = e - 2 * dx;
                    }
                    // Step one x pixel
                    x += stepX;
                    // Step forward with dy (3 dy to account for new slope of 3/2)
                    e = e + 2 * dy;
                }
            }
        }
        else // dx < dy
        {
            // More rise than run, step through y as the primary.
            // Octant, 2, 3, 6, or 7

            // Octant 2 or 7
            if (dx > 0)
            {
                // X steps to the right
                stepX = 1;
                if (dy < 0) // Octant 2
                {
                    printf("Octant 2\n");
                    dy = -dy;
                    y = y - 1;
                    stepY = -1;
                }
                else // Octant 7
                {
                    printf("Octant 7\n");
                    stepY = 1;
                }
            }
            // Octant 3 or 6
            else
            {
                dx = -dx;
                // X steps to the left
                stepX = -1;
                if (dy < 0) // Octant 3
                {
                    printf("Octant 3\n");
                    x = x - 1;
                    dy = -dy;
                    y = y - 1;
                    stepY = -1;
                    printf("Stats: x: %d, dy: %d, y: %d, stepY: %d, dx: %d\n", x, dy, y, stepY, dx);
                }
                else // Octant 6
                {
                    printf("Octant 6\n");
                    stepY = 1;
                }
            }
            // Step through the Bresenham algorithm for (int i = 0; i < dy; i++)

            printf("Maybe running out of dy? %d", dy);
            // Set the initial error
            e = 3 * dx - 2 * dy;
            // Iterate through y
            for (int i = 0; i < dy; i++)
            {
                printf("Maybe running out of dy 2? %d", dy);
                // Set the color of the current pixel
                image_setColor(src, y, x, c);
                // While the error is positive, we need to step up
                if (e > 0)
                {
                    // Step one x pixel
                    x += stepX;
                    // Subtract 1 from e to account for the step left or right.
                    // Becomes 2*dy for the integer math
                    e = e - 2 * dy;
                }
                // Step one y pixel
                y += stepY;
                // Step forward with dx
                e = e + 2 * dx;
            }
        }
    }
}
