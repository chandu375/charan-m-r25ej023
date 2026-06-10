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
            canvas[i][j] = '_';
        }
    }
}

void setPixel(int x, int y) {
    if(x >= 0 && x < COLS && y >= 0 && y < ROWS)
        canvas[y][x] = '*';
}

void displayCanvas() {
    int i, j;

    for(i = 0; i < ROWS; i++) {
        for(j = 0; j < COLS; j++) {
            printf("%c", canvas[i][j]);
        }
        printf("\n");
    }
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
    drawLine(x, y, x + width, y);
    drawLine(x, y, x, y + height);
    drawLine(x + width, y, x + width, y + height);
    drawLine(x, y + height, x + width, y + height);
}

void drawCircle(int xc, int yc, int r) {
    int x, y;

    for(y = yc - r; y <= yc + r; y++) {
        for(x = xc - r; x <= xc + r; x++) {

            int dx = x - xc;
            int dy = y - yc;

            if(dx * dx + dy * dy <= r * r)
                setPixel(x, y);
        }
    }
}

void drawTriangle(int x1, int y1,
                  int x2, int y2,
                  int x3, int y3) {

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
                drawRectangle(s.x1, s.y1,
                              s.width, s.height);
                break;

            case CIRCLE:
                drawCircle(s.x1, s.y1,
                           s.radius);
                break;

            case TRIANGLE:
                drawTriangle(
                    s.x1, s.y1,
                    s.x2, s.y2,
                    s.x3, s.y3
                );
                break;
        }
    }
}

void addObject() {

    Shape s;

    printf("Enter Object ID: ");
    scanf("%d", &s.id);

    printf("1.Line 2.Rectangle 3.Circle 4.Triangle : ");
    int choice;
    scanf("%d", &choice);

    switch(choice) {

        case 1:
            s.type = LINE;

            printf("Enter x1 y1 x2 y2: ");
            scanf("%d%d%d%d",
                  &s.x1, &s.y1,
                  &s.x2, &s.y2);
            break;

        case 2:
            s.type = RECTANGLE;

            printf("Enter x y width height: ");
            scanf("%d%d%d%d",
                  &s.x1, &s.y1,
                  &s.width, &s.height);
            break;

        case 3:
            s.type = CIRCLE;

            printf("Enter centerX centerY radius: ");
            scanf("%d%d%d",
                  &s.x1, &s.y1,
                  &s.radius);
            break;

        case 4:
            s.type = TRIANGLE;

            printf("Enter x1 y1 x2 y2 x3 y3: ");
            scanf("%d%d%d%d%d%d",
                  &s.x1, &s.y1,
                  &s.x2, &s.y2,
                  &s.x3, &s.y3);
            break;

        default:
            printf("Invalid Choice\n");
            return;
    }

    objects[objectCount++] = s;
}

void deleteObject() {

    int id;
    int i, j;

    printf("Enter Object ID to Delete: ");
    scanf("%d", &id);

    for(i = 0; i < objectCount; i++) {

        if(objects[i].id == id) {

            for(j = i; j < objectCount - 1; j++)
                objects[j] = objects[j + 1];

            objectCount--;

            printf("Deleted Successfully\n");
            return;
        }
    }

    printf("Object Not Found\n");
}

void modifyObject() {

    int id;
    int i;

    printf("Enter Object ID to Modify: ");
    scanf("%d", &id);

    for(i = 0; i < objectCount; i++) {

        if(objects[i].id == id) {

            Shape *s = &objects[i];

            switch(s->type) {

                case LINE:
                    printf("Enter new x1 y1 x2 y2: ");
                    scanf("%d%d%d%d",
                          &s->x1, &s->y1,
                          &s->x2, &s->y2);
                    break;

                case RECTANGLE:
                    printf("Enter new x y width height: ");
                    scanf("%d%d%d%d",
                          &s->x1, &s->y1,
                          &s->width, &s->height);
                    break;

                case CIRCLE:
                    printf("Enter new centerX centerY radius: ");
                    scanf("%d%d%d",
                          &s->x1, &s->y1,
                          &s->radius);
                    break;

                case TRIANGLE:
                    printf("Enter new x1 y1 x2 y2 x3 y3: ");
                    scanf("%d%d%d%d%d%d",
                          &s->x1, &s->y1,
                          &s->x2, &s->y2,
                          &s->x3, &s->y3);
                    break;
            }

            printf("Modified Successfully\n");
            return;
        }
    }

    printf("Object Not Found\n");
}

/* ---------------- Main ---------------- */

int main() {

    int choice;

    while(1) {

        printf("\n===== 2D GRAPHICS EDITOR =====\n");
        printf("1. Add Object\n");
        printf("2. Delete Object\n");
        printf("3. Modify Object\n");
        printf("4. Display Picture\n");
        printf("5. Exit\n");

        printf("Enter Choice: ");
        scanf("%d", &choice);

        switch(choice) {

            case 1:
                addObject();
                break;

            case 2:
                deleteObject();
                break;

            case 3:
                modifyObject();
                break;

            case 4:
                redrawCanvas();
                displayCanvas();
                break;

            case 5:
                return 0;

            default:
                printf("Invalid Choice\n");
        }
    }

    return 0;
}
