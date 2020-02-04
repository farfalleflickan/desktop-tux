#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <X11/Xlib.h>
#include <X11/xpm.h>
#include "image.h"
#include "lodepng.h"
#include "string.h"
#include "display.h"

void loadPNG(char *filename, int width, int height, display *disp, image *temp) {

    unsigned error;
    unsigned char *png = 0;
    size_t pngsize;
    LodePNGState state;

    lodepng_state_init(&state);

    error = lodepng_load_file(&png, &pngsize, filename);

    if (NULL == temp->data)
        temp->data = malloc(pngsize);

    strncpy(&temp->filename, filename, strlen(filename));
    temp->width = width;
    temp->height = height;

    unsigned char *tempdata=0;
    if (!error) error = lodepng_decode(&tempdata, &width, &height, &state, png, pngsize);
    if (error) printf("error %u: %s\n", error, lodepng_error_text(error));
    free(png);

    size_t size = width * height * 4;
    uint8_t tmp = 0;
    for (size_t i = 0; i < size; i += 4) {
        tmp = tempdata[i];
        tempdata[i] = tempdata[i + 2];
        tempdata[i + 2] = tmp;
    }

    if (NULL == temp->data) {
        temp->data = malloc(sizeof(unsigned char *)*pngsize);
    }
    temp->data=tempdata;
    if (NULL == temp->x_img)
        temp->x_img = malloc(sizeof (XImage));

    temp->x_img = XCreateImage(disp->myDisplay, disp->myVisual, 32, ZPixmap, 0, &temp->data[0], width, height, 32, 0);
    lodepng_state_cleanup(&state);
}

void drawImage(image *img, window *win, display *disp, GC gc, int src_x, int src_y, int x, int y) {
    XPutImage(disp->myDisplay, win->myWindow, gc, img->x_img, src_x, src_y, x, y, img->width, img->height);
}

void drawXpm(display *disp, image *img, GC gc, int x, int y) {
    
}

void loadXpmToData(char *filename, image *myImage) {
    if (XpmReadFileToData(filename, &myImage->x_data)) {
        fprintf(stderr, "Error reading file\n");
        exit(1);
    }

    strncpy(&myImage->filename, filename, strlen(filename));
}

void XpmDataToPixmap(display *disp, window *win, image *img, XpmAttributes *attr) {
/*
    img->p_map=malloc(sizeof(Pixmap));
    img->p_shape=malloc(sizeof(Pixmap));
*/
    XpmCreatePixmapFromData(disp->myDisplay, win->myWindow, img->x_data, &img->p_map, &img->p_shape, attr);
}

void XpmDataToImage(display *disp, image *img, XpmAttributes *attr) {
    XpmCreateImageFromData(disp->myDisplay, img->x_data, &img->x_img, &img->x_shape, attr);
}
/*
void loadXPMtoImage(display *disp, const char* filename, int width, int height, XpmAttributes* attr, image myImage) {
    if (XpmReadFileToImage(disp->myDisplay, filename, &myImage.x_img, &myImage.x_shape, attr)) {
        fprintf(stderr, "Error reading file\n");
        exit(1);
    }

    strncpy(myImage.filename, filename, strlen(filename));
    myImage.width = width;
    myImage.height = height;
}
 */