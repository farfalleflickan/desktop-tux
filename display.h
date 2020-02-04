#pragma once
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "list.h"

typedef struct display {
    Display* myDisplay;
    Visual* myVisual;
    List* List_myWindows;
    int xi_code, myScreen;
} display;

void newDisplay(display *disp);
void closeDisplay(display *disp);
void drawAllDisplayWindows(display *disp);
void flushDisplay(display *disp);