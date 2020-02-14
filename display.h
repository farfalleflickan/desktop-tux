#pragma once
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "list.h"
#include "screen.h"

typedef struct display {
    Display *myDisplay;
    Visual *defaultVisual;
    List *List_myWindows;
    List *List_myScreen;
    int xi_code, numOfScreens, disp_width, disp_height;
} display;

void newDisplay(display *disp);
void closeDisplay(display *disp);
void drawAllDisplayWindows(display *disp);
void flushDisplay(display *disp);
int getCurrentScreen(display *disp, int x, int y);