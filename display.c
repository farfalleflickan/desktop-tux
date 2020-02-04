#include "display.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>

void newDisplay(display *disp) {
    disp->myDisplay = XOpenDisplay(NULL); // NULL = open defualt display
    if (disp->myDisplay == NULL) { // check that we succesfully opened display
        fprintf(stderr, "Cannot open X display!\n");
        exit(1);
    }

    int xi_code, queryEvent, queryError; // check if XI2 is available 
    if (!XQueryExtension(disp->myDisplay, "XInputExtension", &xi_code, &queryEvent, &queryError)) {
        fprintf(stderr, "X Input extension not available\n");
        exit(1);
    }

    disp->myScreen = DefaultScreen(disp->myDisplay); // set default screen
    disp->myVisual = DefaultVisual(disp->myDisplay, disp->myScreen);
    disp->xi_code = xi_code;
    disp->List_myWindows = newList();
}

void closeDisplay(display *disp) {
    XCloseDisplay(disp->myDisplay);
}

void flushDisplay(display *disp) {
    XFlush(disp->myDisplay);
}

void drawAllDisplayWindows(display *disp) {
    // TO DO
}