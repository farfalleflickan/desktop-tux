#include "eventMask.h"
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/extensions/XInput2.h>
#include <X11/Xutil.h>

int initEventMask(eventMask *myMask){
    myMask->myEventMask = malloc(sizeof(XIEventMask));
    myMask->myEventMask->deviceid = XIAllMasterDevices;
    myMask->myEventMask->mask_len = XIMaskLen(XI_LASTEVENT);
    myMask->myEventMask->mask = calloc(myMask->myEventMask->mask_len, sizeof(char));
    XISetMask(myMask->myEventMask->mask, XI_RawKeyPress);
    XISetMask(myMask->myEventMask->mask, XI_RawKeyRelease);
    XISetMask(myMask->myEventMask->mask, XI_RawButtonPress);
    XISetMask(myMask->myEventMask->mask, XI_RawButtonRelease);
    XISetMask(myMask->myEventMask->mask, XI_RawMotion);
    myMask->init=0;
    return 0;
}

eventMask *newEventMask(){
    eventMask *tempMask=malloc(sizeof(eventMask));
    initEventMask(tempMask);
    return tempMask;
}