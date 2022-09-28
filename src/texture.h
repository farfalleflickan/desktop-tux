#pragma once
#include <X11/Xlib.h>
#include <X11/xpm.h>
#include "display.h"

typedef struct texture {
    unsigned int width, height, tex_format, tex_bpp;
    GLuint myTexture;
    char *filename;
} texture;

void loadTexture(texture *tex, char* fileName);
void renderTexture(texture *tex, int x, int y);