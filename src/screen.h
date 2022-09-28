#pragma once

typedef struct screen {
    int num, width, height, x, y;
} screen;

void newScreen(screen *s, int num, int x, int y, int width, int height);