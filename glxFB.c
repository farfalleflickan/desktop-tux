#include <stdio.h>
#include "glxFB.h"

void initFB(Framebuffer *fb, display disp, int  *attr) {
    int numOfConfigs;
    GLXFBConfig *fbconfigs = glXChooseFBConfig(disp.myDisplay, DefaultScreen(disp.myDisplay), attr, &numOfConfigs);
    fb->fb = 0;
    for (int i = 0; i < numOfConfigs; i++) {
        fb->visual = (XVisualInfo*) glXGetVisualFromFBConfig(disp.myDisplay, fbconfigs[i]);
        if (!fb->visual)
            continue;

        fb->pict  = XRenderFindVisualFormat(disp.myDisplay, fb->visual->visual);
        if (!fb->pict)
            continue;

        fb->fb = fbconfigs[i];
        if (fb->pict->direct.alphaMask > 0) {
            break;
        }
    }

    if (!fb->fb) {
        fprintf(stderr, "No matching FB config found");
    }

    fb->colorMap= XCreateColormap(disp.myDisplay, DefaultRootWindow(disp.myDisplay), fb->visual->visual, AllocNone);
}