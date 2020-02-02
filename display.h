#pragma once
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "list.h"

typedef struct display{
    Display* myDisplay;
    List* List_myWindows;
    int myScreen;
    Visual* myVisual;
    int xi_opcode;
} display;

display  *newDisplay();
void closeDisplay(display *disp);
Display *getDisplay(display *disp);
void drawAllDisplayWindows(display *myDisp);
void flushDisplay(display *disp);