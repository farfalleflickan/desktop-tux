#include <X11/extensions/XInput2.h>
#include <stdio.h>
#include <stdlib.h>
#include "display.h"
#include "window.h"
#include "eventMask.h"
#include "list.h"

Window getRootWindow(display *disp) {
    return DefaultRootWindow(disp->myDisplay);
}

window *newWindow(display *disp, eventMask *myEvent, Window myWindow) {
    window *temp = malloc(sizeof (window));
    temp->myWindow = myWindow;
    temp->myDisplay = disp;
    temp->myEventMask = myEvent;
    return temp;
}

window *newRootWindow(display *disp, eventMask *myEvent) {
    window *myWindow = malloc(sizeof (window));
    myWindow->myWindow = 0;
    myWindow->myWindow = getRootWindow(disp);
    myWindow->myDisplay = disp;
    myWindow->myEventMask = myEvent;
    if (myWindow->myEventMask->init == 0 && myWindow->myWindow != 0) {
        XISelectEvents(disp->myDisplay, myWindow->myWindow, myEvent->myEventMask, 1);
        XSync(disp->myDisplay, False);
        free(myEvent->myEventMask->mask);
    } else {
        fprintf(stderr, "Couldn't create a new window!");
    }
    return myWindow;
}

window *createWindow(display *myDisplay, eventMask *myEvent, int x, int y, int width, int height, int borderWidth, int depth, unsigned int class, Visual *visual, unsigned long valuemask, XSetWindowAttributes *attributes) {
    Display *disp = myDisplay->myDisplay;
    window *temp = malloc(sizeof (window));
    temp->myDisplay = myDisplay;
    temp->myEventMask = myEvent;
    temp->myWindow = XCreateWindow(disp, getRootWindow(myDisplay), x, y, width, height, borderWidth, depth,  class, visual, valuemask, attributes);
    temp->myWinAttr=attributes;
    Atom delWindow = XInternAtom(disp, "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(disp, temp->myWindow, &delWindow, 1);
    XMapWindow(disp, temp->myWindow);
    L_push_back(temp->myDisplay->List_myWindows, sizeof (window), temp);
    return temp;
}

window *createSimpleWindow(display *myDisplay, eventMask *myEvent, int x, int y, int width, int height, int borderSize, unsigned long border, unsigned long background) {
    Display *disp = myDisplay->myDisplay;
    window *temp = malloc(sizeof (window));
    temp->myDisplay = myDisplay;
    temp->myEventMask = myEvent;
    temp->myWindow = XCreateSimpleWindow(disp, getRootWindow(myDisplay), x, y, width, height, borderSize, border, background);
    Atom delWindow = XInternAtom(disp, "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(disp, temp->myWindow, &delWindow, 1);
    XMapWindow(disp, temp->myWindow);
    L_push_back(temp->myDisplay->List_myWindows, sizeof (window), temp);
    return temp;
}

void destroyAllDisplayWindows(display *myDisplay) {
    Display *disp = myDisplay->myDisplay;
    for (int i = 0; i < L_getListSize(myDisplay->List_myWindows); i++) {
        List *tempList = L_getListElem(myDisplay->List_myWindows, i);
        window *temp = (struct window*) tempList->data;
        XDestroyWindow(disp, temp->myWindow);
        fprintf(stdout, "%i\n", i);
    }
}