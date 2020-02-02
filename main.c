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
#include "image.h"

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

    /*
    window *root = newRootWindow(myDisplay, myEventMask);
    window *memes = newSimpleWindow(myDisplay, myEventMask, 0, 0, 100, 100, 0, BlackPixel(myDisplay->myDisplay, DefaultScreen(myDisplay->myDisplay)), WhitePixel(myDisplay->myDisplay, DefaultScreen(myDisplay->myDisplay)));
    window *memes2 = newSimpleWindow(myDisplay, myEventMask, 0, 0, 200, 200, 0, BlackPixel(myDisplay->myDisplay, DefaultScreen(myDisplay->myDisplay)), WhitePixel(myDisplay->myDisplay, DefaultScreen(myDisplay->myDisplay)));
     */
    XSetWindowAttributes winAttr;
    winAttr.background_pixel = BlackPixel(myDisplay->myDisplay, myDisplay->myScreen);
    
    window *memes = createWindow(myDisplay, myEventMask, 0, 0, 512, 512, 0, 0, InputOutput, myDisplay->myVisual, CWBackPixel, &winAttr);

    image *img = loadPNG("test.png", 420, 495, myDisplay);
    GC gc = XCreateGC(myDisplay->myDisplay, memes->myWindow, 0, 0);

    int exitStatus = 0;
    while (exitStatus == 0) {
        exitStatus = inputLoop(myDisplay);
        drawAllDisplayWindows(myDisplay);
        drawImage(img, memes, myDisplay, gc, 0, 0, 0, 0);
        fflush(stdout);
        flushDisplay(myDisplay);
    }
    destroyAllDisplayWindows(myDisplay);
    closeDisplay(myDisplay);
    return 0;
}
