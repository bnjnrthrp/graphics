/*
  Bruce A. Maxwell
  Fall 2014

  Test function 1 for the graphics primitives assignment
  Enterpoop
 */

// edit the include files as necessary
#include "../include/Line.h"
#include "../include/Image.h"
#include "../include/ppmIO.h"
#include "../include/Circle.h"
#include "../include/Ellipse.h"

// draw circles, of various sizes.
int main(int argc, char *argv[])
{
  Image *src;
  Color White;
  Color Red;
  Point p;
  Circle circ;
  Ellipse ellipse;

  color_set(&White, 1.0, 1.0, 1.0);
  color_set(&Red, 0.9, 0.05, 0.05);

  src = image_create(400, 600);

  int octants[] = {0, 0, 0, 0, 0, 0, 0, 0};
  int size = 0;
  int quadrants[] = {0, 0, 0, 0};

  for (int i = 0; i < 8; i++)
  {
    octants[i] = i + 1;
    size++;
    point_set2D(&p, 45 + 70 * i, 200);
    circle_set(&circ, p, 30);
    circle_draw_partial(&circ, src, White, octants, size);
  }

  size = 0;
  for (int i = 0; i < 4; i++)
  {
    quadrants[i] = i + 1;
    size++;
    point_set2D(&p, 45 + 70 * i, 300);
    ellipse_set(&ellipse, p, 50, 25);
    ellipse_draw_partial(&ellipse, src, Red, quadrants, size);
  }
  point_set2D(&p, 50, 50);
  ellipse_set(&ellipse, p, 10, 40);
  ellipse_draw(&ellipse, src, White);

  point_set2D(&p, 340, 60);
  circle_set(&circ, p, 60);
  circle_drawFill(&circ, src, Red);

  circle_set(&circ, p, 60);
  circle_draw(&circ, src, White);

  point_set2D(&p, 360, 330);
  ellipse_set(&ellipse, p, 60, 30);
  ellipse_drawFill(&ellipse, src, Red);

  point_set2D(&p, 365, 330);
  ellipse_set(&ellipse, p, 60, 30);
  ellipse_drawFill(&ellipse, src, White);

  image_write(src, "testCircles.ppm");

  image_free(src);

  return (0);
}
