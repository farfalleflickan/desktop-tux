#pragma once
#include <X11/Xlib.h>
#include "display.h"
#include "window.h"

typedef struct image {
    unsigned int width, height;
    char *data;
    char **x_data;
    Pixmap p_map, p_shape; 
    XImage *x_img, *x_shape;
    GContext myGC;
    char filename;
} image;

void loadPNG(char *filename, int width, int height, display *disp, image *temp);
void loadXpmToData(char *filename, image *myImage);
void XpmDataToPixmap(display *disp, window *win, image *img, XpmAttributes *attr);
void XpmDataToImage(display *disp, image *img, XpmAttributes *attr);

void drawImage(image *img, window *win, display *disp, GC gc, int src_x, int src_y, int x, int y);
void drawXpm(display *disp, image *img, GC gc, int x, int y);
void imageInvertAlpha(display *disp, image *img);