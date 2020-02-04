#if 1
#include <X11/extensions/XInput2.h>
#include <X11/extensions/shape.h>
#include <X11/xpm.h>
#include <X11/Xutil.h>
#include <X11/Xlib.h>
#include <Xm/Xm.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "lodepng.h"
#include "display.h"
#include "window.h"
#include "event.h"
#include "image.h"

int printUsage() {
    printf("TBD");
    exit(0);
}

unsigned long get_posix_clock_time() {
    struct timespec ts;

    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0)
        return (unsigned long) (ts.tv_sec * 1000 + (ts.tv_nsec / 1000000));
    else
        return 0;
}

int main(int argc, char * argv[]) {
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--help")) printUsage();
        else {
            printf("Unexpected argument `%s`\n", argv[i]);
            printUsage();
        }
    }


    display disp;
    newDisplay(&disp);
    window root;
    newRootWindow(&disp, &root);
    image img;
    //loadPNG("test.png", 420, 495, &disp, &img);
    loadXpmToData("test.xpm", &img);


    XVisualInfo vinfo;
    if (!XMatchVisualInfo(disp.myDisplay, disp.myScreen, 32, TrueColor, &vinfo)) {
        printf("No visual found supporting 32 bit color, terminating\n");
        exit(EXIT_FAILURE);
    }
    XSetWindowAttributes winAttr;
    winAttr.override_redirect = True; // makes windows floaty and stuff. perfect for the Goose!
    //winAttr.colormap = XCreateColormap(disp.myDisplay, root.myWindow, vinfo.visual, AllocNone);
    winAttr.colormap = XCreateColormap(disp.myDisplay, root.myWindow, disp.myVisual, AllocNone);
    winAttr.background_pixel = 0;
    winAttr.border_pixel = 0;

    XpmAttributes x_attr;
    x_attr.color_key = XPM_COLOR;
    x_attr.valuemask = XpmColorKey | XpmColorTable;



    window goose;
    //createWindow(&disp, &goose, 0, 0, 512, 512, 0, vinfo.depth, InputOutput, vinfo.visual, CWOverrideRedirect | CWColormap | CWBackPixel | CWBorderPixel, &winAttr);
    createWindow(&disp, &goose, 0, 0, 512, 512, 0, 24, InputOutput, disp.myVisual, CWOverrideRedirect | CWColormap | CWBackPixel | CWBorderPixel, &winAttr);
    mapWindow(&disp, &goose);
    XpmDataToPixmap(&disp, &goose, &img, &x_attr);
    GC gc = XCreateGC(disp.myDisplay, goose.myWindow, 0, 0);
        XSetClipOrigin(disp.myDisplay, gc, 0, 0);
        XSetClipMask(disp.myDisplay, gc, img.p_shape);
        mapWindow(&disp, &goose);
        XSetClipMask(disp.myDisplay, gc, None);
        XCopyArea(disp.myDisplay, img.p_map, goose.myWindow, gc, 0, 0, 420, 495, 0, 0);
        XShapeCombineMask(disp.myDisplay, goose.myWindow, 0, 0, ShapeBounding, img.p_shape, ShapeSet);
    
    XMoveWindow(disp.myDisplay, goose.myWindow, 1600, 300);

    int x = 0, y = 0;
    int exitStatus = 0;

    unsigned long startTime = get_posix_clock_time();
    unsigned long inputTime = startTime;
    for (int i = 0; exitStatus == 0; i++) {
        unsigned long newTime = get_posix_clock_time();
        //drawImage(&img, &goose, &disp, gc, 0, 0, 0, 0);

        XMoveWindow(disp.myDisplay, goose.myWindow, x, y);
                XSetClipOrigin(disp.myDisplay, gc, 0, 0);
                XCopyArea(disp.myDisplay, img.p_map, goose.myWindow, gc, 0, 0, 420, 495, 0, 0);
        if (i % 30 == 0) {
            x++;
        }
        flushDisplay(&disp);
        if (x > XDisplayWidth(disp.myDisplay, disp.myScreen)) {
            x = -210;
        }
        if (newTime - inputTime >= 50) {
            exitStatus = inputLoop(&disp);
            inputTime = get_posix_clock_time();
        }
        startTime = get_posix_clock_time();
        if (18 > startTime - newTime) {
            usleep(18 - (startTime - newTime));
        }
        //flushDisplay(&disp);
    }
    destroyAllDisplayWindows(&disp);
    closeDisplay(&disp);
    return 0;
}
#endif
