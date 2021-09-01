#pragma once

#include "vector.h"
#include "colors.h"

void BressenhamAlgo(int, int, Color);
void putpixel(int, int, Color);
int Round(float);
float x, y, dx, dy;
int steps, k, ptX1, ptY1, ptX2, ptY2;

void BressenhamAlgo(float x1, float yy, float x2, float y2, Color color)
{
    float dx, dy;
    float steps, k;

    dx = abs(x2 - x1);
    dy = abs(y2 - yy);

    float lx, ly;
    if (x2 > x1)
        lx = 1;
    else
        lx = -1;

    if (y2 > yy)
        ly = 1;
    else
        ly = -1;

    float x = x1, y = yy;
    // slope < 1
    if (dx > dy)
    {
        float p = 2 * dy - dx;
        for (float k = 0; k <= dx; k++)
        {
            putpixel(Round(x), Round(y), color);
            if (p < 0)
            {
                x += lx;
                p += 2 * dy;
            }
            else
            {
                x += lx;
                y += ly;
                p += 2 * dy - 2 * dx;
            }
        }
    }
    else
    { //slope > 1
        float p = 2 * dx - dy;
        for (float k = 0; k <= dy; k++)
        {
            putpixel(Round(x), Round(y), color);
            if (p < 0)
            {
                y += ly;
                p += 2 * dx;
            }
            else
            {
                x += lx;
                y += ly;
                p += 2 * dx - 2 * dy;
            }
        }
    }
    // putpixel(x, y, color);
}

int Round(float a)
{
    int b = a + 0.5;
    return b;
}

void draw_polygon(vec4f polygon[n], Color color)
{
    BressenhamAlgo(polygon[0].x, polygon[0].y, polygon[1].x, polygon[1].y, color);
    BressenhamAlgo(polygon[0].x, polygon[0].y, polygon[2].x, polygon[2].y, color);
    BressenhamAlgo(polygon[0].x, polygon[0].y, polygon[4].x, polygon[4].y, color);
    BressenhamAlgo(polygon[1].x, polygon[1].y, polygon[3].x, polygon[3].y, color);
    BressenhamAlgo(polygon[1].x, polygon[1].y, polygon[5].x, polygon[5].y, color);
    BressenhamAlgo(polygon[2].x, polygon[2].y, polygon[3].x, polygon[3].y, color);
    BressenhamAlgo(polygon[5].x, polygon[5].y, polygon[4].x, polygon[4].y, color);
    BressenhamAlgo(polygon[2].x, polygon[2].y, polygon[6].x, polygon[6].y, color);
    BressenhamAlgo(polygon[7].x, polygon[7].y, polygon[6].x, polygon[6].y, color);
    BressenhamAlgo(polygon[4].x, polygon[4].y, polygon[6].x, polygon[6].y, color);
    BressenhamAlgo(polygon[7].x, polygon[7].y, polygon[3].x, polygon[3].y, color);
    BressenhamAlgo(polygon[7].x, polygon[7].y, polygon[5].x, polygon[5].y, color);
}

void putpixel(int x, int y, Color color)
{
    glColor3f(color.r, color.g, color.b);
    glPointSize(2);     // sets the size of points to be drawn (in pixels)
    glBegin(GL_POINTS); // writes pixels on the frame buffer with the current drawing color
    glLoadIdentity();
    glVertex2i(x, y);   //sets vertex
    glEnd();
}