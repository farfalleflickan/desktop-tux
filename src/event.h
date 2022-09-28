#pragma once
#include <X11/XKBlib.h>
#include <X11/extensions/XInput2.h>
#include "display.h"

void print_rawEvent(XIRawEvent *event);
int inputLoop(display *myDisplay);

