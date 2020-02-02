#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <X11/Xlib.h>
#include "image.h"
#include "lodepng.h"
#include "string.h"
#include "display.h"

image * loadPNG(const char* filename, int width, int height, display *myDisplay) {
    image *temp = malloc(sizeof (image));
    unsigned error;
    unsigned char *png = 0;
    size_t pngsize;
    LodePNGState state;

    lodepng_state_init(&state);

    error = lodepng_load_file(&png, &pngsize, filename);
    temp->data = malloc(sizeof (pngsize));
    temp->filename = calloc(strlen(filename), sizeof (char));
    temp->data = 0;
    strncpy(temp->filename, filename, strlen(filename));
    temp->width = width;
    temp->height = height;
    if (!error) error = lodepng_decode(&temp->data, &temp->width, &temp->height, &state, png, pngsize);
    if (error) printf("error %u: %s\n", error, lodepng_error_text(error));

    free(png);

    size_t size = width * height * 4;
    uint8_t tmp = 0;
    for (size_t i = 0; i < size; i += 4) {
        tmp = temp->data[i];
        temp->data[i] = temp->data[i + 2];
        temp->data[i + 2] = tmp;
    }

    temp->myImage = XCreateImage(myDisplay->myDisplay, myDisplay->myVisual, 24, ZPixmap, 0, (char *) &temp->data[0], width, height, 32, 0);
    lodepng_state_cleanup(&state);
    return temp;
}

void drawImage(image *img, window *win, display *disp, GC gc, int src_x, int src_y, int x, int y) {
    XPutImage(disp->myDisplay, win->myWindow, gc, img->myImage, src_x, src_y, x, y, img->width, img->height);
}