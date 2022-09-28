#include <X11/extensions/Xrandr.h>
#include <stdio.h>
#include <stdlib.h>
#include "display.h"
#include "list.h"
#include "screen.h"

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

    XRRScreenResources *xorg = XRRGetScreenResources(disp->myDisplay, DefaultRootWindow(disp->myDisplay));
    Screen *temp_scr = XDefaultScreenOfDisplay(disp->myDisplay);
    disp->disp_width = temp_scr->width;
    disp->disp_height = temp_scr->height;
    disp->defaultVisual = DefaultVisual(disp->myDisplay, 0);
    disp->xi_code = xi_code;
    disp->List_myWindows = newList();
    disp->List_myScreen = newList();
    int numDisplays=0;
    for (int i = 0; i < xorg->noutput; i++) {
        XRROutputInfo *output = XRRGetOutputInfo(disp->myDisplay, xorg, xorg->outputs[i]);
        if (output->crtc) {
            XRRCrtcInfo *crtc = XRRGetCrtcInfo(disp->myDisplay, xorg, output->crtc);
            screen *scr = malloc(sizeof (screen));
            scr->num = numDisplays;
            scr->x = crtc->x;
            scr->y = crtc->y;
            scr->width = crtc->width;
            scr->height = crtc->height;
            //printf("%i\t%i\t%i\t%i\t%i\n", scr->num, scr->x, scr->y, scr->width, scr->height);
            L_push_back(disp->List_myScreen, sizeof (screen), scr);
            XRRFreeCrtcInfo(crtc);
			numDisplays++;
        }
        XRRFreeOutputInfo(output);
    }
    if (numDisplays == 0) {
        disp->numOfScreens = 1;
    } else {
        disp->numOfScreens = numDisplays;
        //mapScreenDisplay(disp);
    }
}

int getCurrentScreen(display *disp, int x, int y) {
    if (disp->numOfScreens > 1) {
        int temp = disp->numOfScreens;
        for (int i = 1; i <= temp; i++) {
            screen *scr = (screen *) L_getListElem(disp->List_myScreen, (i - 1))->data;
            //printf("i:%i\tx:%i\ty:%i\tscr_x:%i\tscr_y:%i\tscr_width:%i\tscr_height:%i\n", i, x, y, scr->x, scr->y, scr->width, scr->height);
            if ((x >= scr->x && x <= (scr->x + scr->width)) && (y >= scr->y && y <= (scr->y + scr->height))) {
                //printf("RETURNED %i\n", i);
                return i;
            }
        }
        return 0;
    }
    return 1;
}

void closeDisplay(display *disp) {
    freeList(disp->List_myWindows);
    freeList(disp->List_myScreen);
    XCloseDisplay(disp->myDisplay);
}

void flushDisplay(display *disp) {
    XFlush(disp->myDisplay);
}

int isCompositorRunning(Display *dpy, int screen) {
    char prop_name[20];
    snprintf(prop_name, 20, "_NET_WM_CM_S%d", screen);
    Atom prop_atom = XInternAtom(dpy, prop_name, False);
    return XGetSelectionOwner(dpy, prop_atom) != None;
}

/*
void mapScreenDisplay(display *disp) {
    List myL=disp->List_myScreen;
    for (int i = 0; i < L_getListSize(myL); i++) {
        screen *scr = (screen) L_getListElem(myL, i);
        
    }
}
 */

void drawAllDisplayWindows(display *disp) {
    // TO DO
}
