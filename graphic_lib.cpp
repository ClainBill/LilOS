#include "graphic_lib.h"
#include "main.h"

#include <stdlib.h>
#include <iostream>

void drawPixel(int x, int y, uint32_t colour){
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT){
        framebuffer[y * WIDTH + x] = colour;
    }
}

void drawLine(int x1, int y1, int x2, int y2, uint32_t colour){
    // get absolute diff
    int dx = abs(x2 - x1);
    int sx = (x1 < x2) ? 1 : -1; // step direction for x

    int dy = -abs(y2 - y1);
    int sy = (y1 < y2) ? 1 : -1; // step direction for y

    int err = dx + dy; // error margin
    int e2; //temp error value

    while(1){
        // bounding check
        if (x1 < 0 || x1 >= WIDTH || y1 < 0 || y1 >= HEIGHT)
            break;
        
        framebuffer[y1 * WIDTH + x1] = colour;

        if (x1 == x2 && y1 == y2)
            break;

        e2 = 2* err;

        // adjust error and step x
        if (e2 >= dy){
            err += dy;
            x1 += sx;
        }

        // adjust error and step y
        if (e2 <= dx){
            err += dx;
            y1 += sy;
        }
    }
}

void drawRect(int x1, int y1, int x2, int y2, uint32_t colour, bool fill){
    if (fill){
        for (int j = y1; j < y2; j++){
            for (int i = x1; i < x2; i++){
                framebuffer[j * WIDTH + i] = colour;
            }
        }
    }
    else {
        drawLine(x1, y1, x2, y1, colour);
        drawLine(x2, y1, x2, y2, colour);
        drawLine(x2, y2, x1, y2, colour);
        drawLine(x1, y2, x1, y1, colour);
    }
}

void drawCircle(int xc, int yc, int r, uint32_t colour, bool fill){
    // Set the cursor to x = 0 y = -r (relative to midpoint)
    int x = 0;
    int y = -r;
    int p = -r; // p is the decision of if we move y or not

    while (x <= -y){
        // If the current pixel is inside the circle, we move y and increase p
        if (p > 0){
            y++;
            p += 2 * (x + y) + 1;
        }
        else{ // Otherwise just increase p
            p += 2 * x + 1;
        }

        // Draw pixels mirrored in each symmetrical octet of the circle
        // We want to fill if flagged for it and the y isn't min/max radius
        if (fill){
            drawLine(xc + x, yc + y, xc - x, yc + y, colour); // TOP
            drawLine(xc + y, yc - x, xc - y, yc - x, colour); // TOP MIDDLE
            drawLine(xc + y, yc + x, xc - y, yc + x, colour); // BOTTOM MIDDLE
            drawLine(xc + x, yc - y, xc - x, yc - y, colour); // BOTTOM
        } else {
            drawPixel(xc + x, yc + y, colour); // TOP
            drawPixel(xc - x, yc + y, colour);
            drawPixel(xc + x, yc - y, colour); // BOTTOM
            drawPixel(xc - x, yc - y, colour);
            drawPixel(xc + y, yc + x, colour); // BOTTOM MIDDLE
            drawPixel(xc - y, yc + x, colour);
            drawPixel(xc + y, yc - x, colour); // TOP MIDDLE
            drawPixel(xc - y, yc - x, colour);
        }

        x++;
    }
}