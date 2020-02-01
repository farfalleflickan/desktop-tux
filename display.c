#include "display.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>

display *newDisplay() {
    display *new=malloc(sizeof(display));
    new->myDisplay = XOpenDisplay(NULL); // NULL = open defualt display
    if (new->myDisplay == NULL) { // check that we succesfully opened display
        fprintf(stderr, "Cannot open X display!\n");
        return NULL;
    }

    int xi_opcode, queryEvent, queryError; // check if XI2 is available 
    if (!XQueryExtension(new->myDisplay, "XInputExtension", &xi_opcode, &queryEvent, &queryError)) {
        fprintf(stderr, "X Input extension not available\n");
        return NULL;
    }

    new->myScreen = DefaultScreen(new->myDisplay); // set default screen
    new->xi_opcode=xi_opcode;
    new->List_myWindows=newList();
    return new;
}

void closeDisplay(display *disp) {
    XCloseDisplay(disp->myDisplay);
}

Display *getDisplay(display *disp) {
    return disp->myDisplay;
}

void flushDisplay(display *disp){
    XFlush(disp->myDisplay);
}

void drawAllDisplayWindows(display *myDisp) {
    // TO DO
}