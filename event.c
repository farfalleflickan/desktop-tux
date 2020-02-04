#include "event.h"
#include "display.h"
#include "window.h"
#include <stdio.h>
#include <string.h>

void print_rawEvent(XIRawEvent *event) {
    int i;
    int lasti = -1;
    double *val;

    fprintf(stdout, "EVENT type %d ", event->evtype);
    fprintf(stdout, "device %d %d ", event->deviceid, event->sourceid);
    fprintf(stdout, "detail %d ", event->detail);
    fprintf(stdout, "valuators ");

    /* Get the index of the last valuator that is set */
    for (i = 0; i < event->valuators.mask_len * 8; i++) {
        if (XIMaskIsSet(event->valuators.mask, i)) {
            lasti = i;
        }
    }

    /* Print each valuator's value, nan if the valuator is not set. */
    val = event->valuators.values;
    for (i = 0; i <= lasti; i++) {
        if (XIMaskIsSet(event->valuators.mask, i)) {
            fprintf(stdout, " %.2f", *val++);
        } else {
            fprintf(stdout, " nan\n");
        }
    }
}

int inputLoop(display *myDisplay) {
    Display *disp = myDisplay->myDisplay;
    XEvent event;
    XGenericEventCookie *cookie = (XGenericEventCookie*) & event.xcookie;
    while (XPending(disp)) {
        XNextEvent(disp, &event);
        
        if (event.type == ClientMessage)
            return 1;

        if (XGetEventData(disp, cookie) && cookie->type == GenericEvent && cookie->extension == myDisplay->xi_code) {
            switch (cookie->evtype) {
                    //case XI_RawKeyRelease: 
                case XI_RawKeyPress:
                {
                    XIRawEvent *ev = cookie->data;

                    // Ask X what it calls that key
                    KeySym s = XkbKeycodeToKeysym(disp, ev->detail, 0, 0);
                    if (NoSymbol != s) {
                        char *str = XKeysymToString(s);
                        if (NULL != str && strcmp("F12", str) == 0) {
                            return 1;
                        } else if (NULL != str) {
                            //fprintf(stdout, "%s\n", str);
                        } else {
                            //fprintf(stderr, "Unparsable key!\n");
                        }
                    } else {
                        //fprintf(stderr, "Unknown key!!\n");
                    }
                    break;
                }
                case XI_RawMotion: {
                    Window root_ret, child_ret;
                    int root_x, root_y;
                    int win_x, win_y;
                    unsigned int mask;
                    XIRawEvent *re = cookie->data;
                    XQueryPointer(disp, DefaultRootWindow(disp), &root_ret, &child_ret, &root_x, &root_y, &win_x, &win_y, &mask);
                    fprintf(stdout, "raw %g,%g root %d,%d\n", re->raw_values[0], re->raw_values[1], root_x, root_y);
                    break;
                }
                    //case XI_RawButtonRelease:
                case XI_RawButtonPress:
                {
                    XIRawEvent *ev = cookie->data;
                    break;
                }
            }
        }
        XFreeEventData(disp, cookie);
        break;
    }
    return 0;
}