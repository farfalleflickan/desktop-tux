#pragma once
#include "display.h"
#include "eventMask.h"

typedef struct window {
    Window myWindow;
    display *myDisplay;
    eventMask *myEventMask;
    
} window;

Window getRootWindow();
window *newWindow(display *disp, eventMask *myEvent, Window myWindow);
window *newRootWindow(display *disp, eventMask *myEvent);
window *newSimpleWindow(display *myDisplay, eventMask *myEvent, int x, int y, int width, int height, int borderSize, unsigned long border, unsigned long background);
void destroyAllDisplayWindows(display *myDisplay);