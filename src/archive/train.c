#include "../include/Line.h"
#include "../include/Image.h"
#include "../include/ppmIO.h"
#include "../include/Circle.h"
#include "../include/Ellipse.h"
#include "../include/Polyline.h"

// draw circles, of various sizes.
int main(int argc, char *argv[])
{
    const int nPoints = 100;

    Image *src;
    Point p[nPoints];
    Point p1;
    Point p2;
    Circle circ;
    Ellipse ellipse;
    Line line;
    Polyline chimney;
    Polyline *body;
    Polyline *cockpit;
    Color White;
    Color Red;
    Color DarkGray;
    Color LightGray;
    Color Black;
    Color Blue;
    Color DarkBlue;

    color_set(&White, 1.0, 1.0, 1.0);
    color_set(&Red, 0.9, 0.05, 0.05);
    color_set(&DarkGray, .16, .16, .16);
    color_set(&LightGray, .85, .85, .85);
    color_set(&Black, .05, .05, .05);
    color_set(&Blue, .12, .21, .67);
    color_set(&DarkBlue, .09, .15, .50);

    src = image_create(800, 1200);
    image_fillc(src, LightGray);

    int octants[] = {0, 0, 0, 0, 0, 0, 0, 0};
    int size = 0;
    int quadrants[] = {0, 0, 0, 0};

    // Make the body of the train
    point_set2D(&p1, 870, 311);
    point_set2D(&p2, 420, 255);
    line_set(&line, p1, p2);
    line_draw(&line, src, Blue);
    point_set2D(&p1, 420, 440);
    point_set2D(&p2, 880, 500);
    line_set(&line, p1, p2);
    line_draw(&line, src, Blue);

    point_set2D(&p1, 420, 348);
    ellipse_set(&ellipse, p1, 40, 94);
    quadrants[0] = 2;
    quadrants[1] = 3;
    ellipse_draw_partial(&ellipse, src, Blue, quadrants, 2);

    // set octants to the lower half
    // Back wheels
    octants[0] = 5;
    octants[1] = 6;
    octants[2] = 7;
    octants[3] = 8;
    for (int i = 0; i < 4; i++)
    {
        point_set2D(&p1, 765 - 109 * i, 513 - 15 * i);
        circle_set(&circ, p1, 50);
        circle_draw_partial(&circ, src, Black, octants, 4);
    }
    octants[4] = 1;
    point_set2D(&p1, 874, 528);
    circle_set(&circ, p1, 50);
    circle_draw_partial(&circ, src, Black, octants, 5);

    // Wheels
    for (int i = 0; i < 5; i++)
    {
        point_set2D(&p1, 828 - 109 * i, 548 - 15 * i);
        ellipse_set(&ellipse, p1, 44, 54);
        ellipse_drawFill(&ellipse, src, DarkGray);

        point_set2D(&p1, p1.val[0] - 4, p1.val[1] - 4);
        ellipse_set(&ellipse, p1, 40, 50);
        ellipse_drawFill(&ellipse, src, LightGray);
    }

    // Chimney stack outlet
    point_set2D(&p2, 880, 220);
    ellipse_set(&ellipse, p2, 20, 10);
    ellipse_draw(&ellipse, src, DarkGray);

    // Chimney stack
    point_set2D(&p1, 860, 221);
    point_set2D(&p2, 860, 309);
    line_set(&line, p1, p2);
    line_draw(&line, src, DarkGray);

    point_set2D(&p2, 899, 222);
    point_set2D(&p1, 899, 313);
    line_set(&line, p1, p2);
    line_draw(&line, src, DarkGray);

    // Face
    point_set2D(&p1, 880, 404);
    ellipse_set(&ellipse, p1, 60, 95);
    ellipse_draw(&ellipse, src, DarkGray);

    // Ribs of train
    point_set2D(&p1, 733, 387);
    ellipse_set(&ellipse, p1, 40, 94);
    quadrants[0] = 2;
    quadrants[1] = 3;
    ellipse_draw_partial(&ellipse, src, Red, quadrants, 2);

    point_set2D(&p1, 600, 370);
    ellipse_set(&ellipse, p1, 40, 94);
    ellipse_draw_partial(&ellipse, src, Red, quadrants, 2);

    point_set2D(&p1, 467, 353);
    ellipse_set(&ellipse, p1, 40, 94);
    ellipse_draw_partial(&ellipse, src, Red, quadrants, 2);

    // Pilot house ceiling
    point_set2D(&p1, 466, 259);
    point_set2D(&p2, 466, 222);
    line_set(&line, p1, p2);
    line_draw(&line, src, Blue);

    // Pilot house body
    polyline_init(&body);
    point_set2D(&(p[0]), 469, 220);
    point_set2D(&(p[1]), 339, 261);
    point_set2D(&(p[2]), 258, 251);
    point_set2D(&(p[3]), 388, 210);
    point_set2D(&(p[4]), 469, 220);
    polyline_set(&body, 5, &(p[0]));
    polyline_draw(&body, src, Black);

    point_set2D(&(p[0]), 416, 442);
    point_set2D(&(p[1]), 339, 460);
    point_set2D(&(p[2]), 261, 450);
    point_set2D(&(p[3]), 261, 250);
    polyline_set(&body, 4, &(p[0]));
    polyline_draw(&body, src, Blue);

    line_set2D(&line, 339, 460, 339, 261);
    line_draw(&line, src, Blue);

        image_write(src, "ThomasTheTankEngine.ppm");

    image_free(src);

    return (0);
}
