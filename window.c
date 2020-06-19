#include <X11/extensions/XInput2.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "display.h"
#include "window.h"
#include "list.h"
#include "glxFB.h"
#include "utils.h"

Window getRootWindow(display *disp) {
    return DefaultRootWindow(disp->myDisplay);
}

void newRootWindow(display *disp, window *win) {
    win->myXID[0] = '\0';
    win->myWindow = getRootWindow(disp);
    win->myDisplay = disp;
    win->mask.deviceid = XIAllMasterDevices;
    win->mask.mask_len = XIMaskLen(XI_LASTEVENT);
    win->mask.mask = calloc(win->mask.mask_len, sizeof (unsigned char));
    XISetMask(win->mask.mask, XI_RawKeyPress);
    XISetMask(win->mask.mask, XI_RawButtonPress);
    //XISetMask(win->mask.mask, XI_RawMotion);
    XISelectEvents(disp->myDisplay, win->myWindow, &win->mask, 1);
    XSync(disp->myDisplay, False);
    free(win->mask.mask);
    win->x = 0;
    win->y = 0;
    win->width = 0;
    win->height = 0;
}

void createWindow(display *myDisplay, window *win, int x, int y, int width, int height, int borderWidth, int depth, unsigned int class, Visual *visual, unsigned long valuemask, XSetWindowAttributes *attributes) {
    Display *disp = myDisplay->myDisplay;
    win->myXID[0] = '\0';
    win->myDisplay = myDisplay;
    win->myWindow = XCreateWindow(disp, getRootWindow(myDisplay), x, y, width, height, borderWidth, depth, class, visual, valuemask, attributes);
    sprintf(win->myXID, "%li", win->myWindow);
    win->myWinAttr = attributes;
    Atom delWindow = XInternAtom(disp, "WM_DELETE_WINDOW", 0);
    //XSelectInput(disp, win->myWindow, ExposureMask | KeyPressMask | XI_RawMotionMask | StructureNotifyMask | ResizeRedirectMask | PropertyChangeMask);
    XSetWMProtocols(disp, win->myWindow, &delWindow, 1);
    win->x = x;
    win->y = y;
    win->width = width;
    win->height = height;
}

void createSimpleWindow(display *display, window *win, int x, int y, int width, int height, int borderSize, unsigned long border, unsigned long background) {
    win->myXID[0] = '\0';
    Display *disp = display->myDisplay;
    win->myDisplay = display;
    win->myWindow = XCreateSimpleWindow(disp, getRootWindow(display), x, y, width, height, borderSize, border, background);
    sprintf(win->myXID, "%li", win->myWindow);
    Atom delWindow = XInternAtom(disp, "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(disp, win->myWindow, &delWindow, 1);
    win->x = x;
    win->y = y;
    win->width = width;
    win->height = height;
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

void mapWindow(display *disp, window *w) {
    L_push_back(disp->List_myWindows, sizeof (&w), w);
    XMapWindow(disp->myDisplay, w->myWindow);
}

void initRenderContext(window *win, display *disp, Framebuffer fb) {
    win->renderContext = glXCreateContext(disp->myDisplay, fb.visual, NULL, GL_TRUE);     
    XSync(disp->myDisplay, False);
    if (win->renderContext) {
        printf("Created GL 3.0 context\n");
    }
    
    if (!glXMakeCurrent(disp->myDisplay, win->myWindow, win->renderContext)) {
        fatalError("glXMakeCurrent failed for window\n");
    }
}
