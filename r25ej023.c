#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROWS 30
#define COLS 60
#define MAX_OBJECTS 100

char canvas[ROWS][COLS];

typedef enum {
    LINE,
    RECTANGLE,
    CIRCLE,
    TRIANGLE
} ShapeType;

typedef struct {
    int id;
    ShapeType type;

    int x1, y1, x2, y2, x3, y3;
    int width, height;
    int radius;
} Shape;

Shape objects[MAX_OBJECTS];
int objectCount = 0;

/* ---------------- Canvas Functions ---------------- */

void clearCanvas() {
    int i, j;
    for(i = 0; i < ROWS; i++) {
        for(j = 0; j < COLS; j++) {
            // Using spaces instead of '_' internally prevents distortion
            canvas[i][j] = ' '; 
        }
    }
}

void setPixel(int x, int y) {
    if(x >= 0 && x < COLS && y >= 0 && y < ROWS)
        canvas[y][x] = '*';
}

void displayCanvas() {
    int i, j;
    
    // Print Top Frame Boundary
    for(j = 0; j < COLS + 2; j++) printf("-");
    printf("\n");

    for(i = 0; i < ROWS; i++) {
        printf("|"); // Left border
        for(j = 0; j < COLS; j++) {
            printf("%c", canvas[i][j]);
        }
        printf("|\n"); // Right border
    }

    // Print Bottom Frame Boundary
    for(j = 0; j < COLS + 2; j++) printf("-");
    printf("\n");
}

/* ---------------- Drawing Functions ---------------- */

void drawLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;

    while(1) {
        setPixel(x1, y1);

        if(x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;

        if(e2 > -dy) {
            err -= dy;
            x1 += sx;
        }

        if(e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void drawRectangle(int x, int y, int width, int height) {
    // Draws standard hollow boundaries cleanly
    drawLine(x, y, x + width, y);
    drawLine(x, y, x, y + height);
    drawLine(x + width, y, x + width, y + height);
    drawLine(x, y + height, x + width, y + height);
}

void drawCircle(int xc, int yc, int r) {
    // Standard Midpoint Circle Algorithm for HOLLOW rendering
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    while (y >= x) {
        setPixel(xc + x, yc + y);
        setPixel(xc - x, yc + y);
        setPixel(xc + x, yc - y);
        setPixel(xc - x, yc - y);
        setPixel(xc + y, yc + x);
        setPixel(xc - y, yc + x);
        setPixel(xc + y, yc - x);
        setPixel(xc - y, yc - x);
        x++;

        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
    }
}

void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    drawLine(x1, y1, x2, y2);
    drawLine(x2, y2, x3, y3);
    drawLine(x3, y3, x1, y1);
}

/* ---------------- Object Management ---------------- */

void redrawCanvas() {
    int i;
    clearCanvas();

    for(i = 0; i < objectCount; i++) {
        Shape s = objects[i];

        switch(s.type) {
            case LINE:
                drawLine(s.x1, s.y1, s.x2, s.y2);
                break;
            case RECTANGLE:
                drawRectangle(s.x1, s.y1, s.width, s.height);
                break;
            case CIRCLE:
