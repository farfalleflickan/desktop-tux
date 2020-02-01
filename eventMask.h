#pragma once
#include <X11/extensions/XInput2.h>

typedef struct eventMask{
    XIEventMask *myEventMask;
    int init;
} eventMask;

int initEventMask(eventMask *myMask);
eventMask *newEventMask();