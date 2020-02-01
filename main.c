#include <X11/XKBlib.h>
#include <X11/extensions/XInput2.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lodepng.h"
#include "display.h"
#include "window.h"
#include "eventMask.h"
#include "event.h"

int printUsage() {
    printf("TBD");
    exit(0);
}

int main(int argc, char * argv[]) {
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--help")) printUsage();
        else {
            printf("Unexpected argument `%s`\n", argv[i]);
            printUsage();
        }
    }
    display * myDisplay = newDisplay();
    eventMask *myEventMask = newEventMask();
    window *root = newRootWindow(myDisplay, myEventMask);
    window *memes = newSimpleWindow(myDisplay, myEventMask, 0, 0, 100, 100, 0, BlackPixel(myDisplay->myDisplay, DefaultScreen(myDisplay->myDisplay)), WhitePixel(myDisplay->myDisplay, DefaultScreen(myDisplay->myDisplay)));
    window *memes2 = newSimpleWindow(myDisplay, myEventMask, 0, 0, 200, 200, 0, BlackPixel(myDisplay->myDisplay, DefaultScreen(myDisplay->myDisplay)), WhitePixel(myDisplay->myDisplay, DefaultScreen(myDisplay->myDisplay)));
    
    int exitStatus=0;
    while (exitStatus==0) {
        exitStatus=inputLoop(myDisplay);
        drawAllDisplayWindows(myDisplay);
        fflush(stdout);
    }
    destroyAllDisplayWindows(myDisplay);
    closeDisplay(myDisplay);
    return 0;
}
