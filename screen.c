#include "screen.h"

void newScreen(screen *s, int num, int x, int y, int width, int height) {
    s->num = num;
    s->x = x;
    s->y = y;
    s->width = width;
    s->height = height;
}

