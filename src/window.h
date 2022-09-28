#pragma once
#include "display.h"
#include "glxFB.h"
#include <X11/extensions/XInput2.h>
#include <GL/glx.h>

typedef struct window {
    Window myWindow;
    GLXContext renderContext;
    display *myDisplay;
    XIEventMask mask;
    XSetWindowAttributes *myWinAttr;
    char myXID[16];
    int x, y, width, height;
} window;

void newWindow(display *disp, Window *win);
void newRootWindow(display *disp,window *win);
void createWindow(display *myDisplay, window *temp, int x, int y, int width, int height, int borderWidth, int depth, unsigned int class, Visual *visual, unsigned long valuemask, XSetWindowAttributes *attributes);
void createSimpleWindow(display *display, window *temp, int x, int y, int width, int height, int borderSize, unsigned long border, unsigned long background);
Window getRootWindow();
void destroyAllDisplayWindows(display *disp);
void mapWindow(display *disp, window *w);
void initRenderContext(window *win, display *disp, Framebuffer fb);