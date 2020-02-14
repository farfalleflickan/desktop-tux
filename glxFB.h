#pragma once
#include <X11/extensions/Xrender.h>
#include <GL/glx.h>
#include "display.h"

typedef struct Framebuffer {
    GLXFBConfig fb;
    XVisualInfo *visual;
    XRenderPictFormat *pict;
    Colormap colorMap;
} Framebuffer;

void initFB(Framebuffer *fb, display disp, int *attr);