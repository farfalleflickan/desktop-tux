#pragma once
#include <X11/Xlib.h>
#include "display.h"
#include "window.h"

typedef struct image {
    unsigned int width, height;
    unsigned char * data;
    XImage *myImage;
    char* filename;
} image;

image * loadPNG(const char* filename, int width, int height, display *myDisplay);

void drawImage(image *img, window *win, display *disp, GC gc, int src_x, int src_y, int x, int y);