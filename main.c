
#include <X11/extensions/XInput2.h>
#include <X11/extensions/Xrender.h>
#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glxext.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>    
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <time.h>

#include "lodepng.h"
#include "display.h"
#include "window.h"
#include "event.h"
#include "image.h"
#include "glxFB.h"
#include "utils.h"

int printUsage() {
    printf("TBD");
    exit(0);
}



GLfloat cube_vertices[][8] = {
    /*  X     Y     Z   Nx   Ny   Nz    S    T */
    {-1.0, -1.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0}, // 0
    { 1.0, -1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0}, // 1
    { 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0}, // 2
    {-1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0}, // 3

    { 1.0, -1.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0}, // 4
    {-1.0, -1.0, -1.0, 0.0, 0.0, -1.0, 1.0, 0.0}, // 5
    {-1.0, 1.0, -1.0, 0.0, 0.0, -1.0, 1.0, 1.0}, // 6
    { 1.0, 1.0, -1.0, 0.0, 0.0, -1.0, 0.0, 1.0}, // 7

    {-1.0, -1.0, -1.0, -1.0, 0.0, 0.0, 0.0, 0.0}, // 5
    {-1.0, -1.0, 1.0, -1.0, 0.0, 0.0, 1.0, 0.0}, // 0
    {-1.0, 1.0, 1.0, -1.0, 0.0, 0.0, 1.0, 1.0}, // 3
    {-1.0, 1.0, -1.0, -1.0, 0.0, 0.0, 0.0, 1.0}, // 6

    { 1.0, -1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0}, // 1
    { 1.0, -1.0, -1.0, 1.0, 0.0, 0.0, 1.0, 0.0}, // 4
    { 1.0, 1.0, -1.0, 1.0, 0.0, 0.0, 1.0, 1.0}, // 7
    { 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0}, // 2

    {-1.0, -1.0, -1.0, 0.0, -1.0, 0.0, 0.0, 0.0}, // 5
    { 1.0, -1.0, -1.0, 0.0, -1.0, 0.0, 1.0, 0.0}, // 4
    { 1.0, -1.0, 1.0, 0.0, -1.0, 0.0, 1.0, 1.0}, // 1
    {-1.0, -1.0, 1.0, 0.0, -1.0, 0.0, 0.0, 1.0}, // 0

    {-1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0}, // 3
    { 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0}, // 2
    { 1.0, 1.0, -1.0, 0.0, 1.0, 0.0, 1.0, 1.0}, // 7
    {-1.0, 1.0, -1.0, 0.0, 1.0, 0.0, 0.0, 1.0}, // 6
};

static void draw_cube(void) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, sizeof (GLfloat) * 8, &cube_vertices[0][0]);
    glNormalPointer(GL_FLOAT, sizeof (GLfloat) * 8, &cube_vertices[0][3]);
    glTexCoordPointer(2, GL_FLOAT, sizeof (GLfloat) * 8, &cube_vertices[0][6]);

    glDrawArrays(GL_QUADS, 0, 24);
}

float const light0_dir[] = {0, 1, 0, 0};
float const light0_color[] = {78. / 255., 80. / 255., 184. / 255., 1};

float const light1_dir[] = {-1, 1, 1, 0};
float const light1_color[] = {255. / 255., 220. / 255., 97. / 255., 1};

float const light2_dir[] = {0, -1, 0, 0};
float const light2_color[] = {31. / 255., 75. / 255., 16. / 255., 1};

static void redrawTheWindow(Display *Xdisplay, GLXWindow glX_window_handle, float width, float height) {
    float const aspect = (float) width / (float) height;

    static float a = 0;
    static float b = 0;
    static float c = 0;

    glDrawBuffer(GL_BACK);

    glViewport(0, 0, width, height);

    // Clear with alpha = 0.0, i.e. full transparency
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-aspect, aspect, -1, 1, 2.5, 10);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glLightfv(GL_LIGHT0, GL_POSITION, light0_dir);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_color);

    glLightfv(GL_LIGHT1, GL_POSITION, light1_dir);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_color);

    glLightfv(GL_LIGHT2, GL_POSITION, light2_dir);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_color);

    glTranslatef(0., 0., -5.);

    glRotatef(a, 1, 0, 0);
    glRotatef(b, 0, 1, 0);
    glRotatef(c, 0, 0, 1);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glColor4f(1., 1., 1., 0.5);

    glCullFace(GL_FRONT);
    draw_cube();
    glCullFace(GL_BACK);
    draw_cube();

    a = fmod(a + 0.1, 360.);
    b = fmod(b + 0.5, 360.);
    c = fmod(c + 0.25, 360.);

    struct timespec Ta, Tb;

    clock_gettime(CLOCK_MONOTONIC_RAW, &Ta);
    glXSwapBuffers(Xdisplay, glX_window_handle);
    clock_gettime(CLOCK_MONOTONIC_RAW, &Tb);

    // fprintf(stderr, "glXSwapBuffers returned after %f ms\n", 1e3*((double)Tb.tv_sec + 1e-6*(double)Tb.tv_nsec) - 1e3*((double)Ta.tv_sec + 1e-6*(double)Ta.tv_nsec));	
}

static void describe_fbconfig(Display *Xdisplay, GLXFBConfig fbconfig) {
    int doublebuffer;
    int red_bits, green_bits, blue_bits, alpha_bits, depth_bits;

    glXGetFBConfigAttrib(Xdisplay, fbconfig, GLX_DOUBLEBUFFER, &doublebuffer);
    glXGetFBConfigAttrib(Xdisplay, fbconfig, GLX_RED_SIZE, &red_bits);
    glXGetFBConfigAttrib(Xdisplay, fbconfig, GLX_GREEN_SIZE, &green_bits);
    glXGetFBConfigAttrib(Xdisplay, fbconfig, GLX_BLUE_SIZE, &blue_bits);
    glXGetFBConfigAttrib(Xdisplay, fbconfig, GLX_ALPHA_SIZE, &alpha_bits);
    glXGetFBConfigAttrib(Xdisplay, fbconfig, GLX_DEPTH_SIZE, &depth_bits);

    fprintf(stderr, "FBConfig selected:\n"
            "Doublebuffer: %s\n"
            "Red Bits: %d, Green Bits: %d, Blue Bits: %d, Alpha Bits: %d, Depth Bits: %d\n",
            doublebuffer == True ? "Yes" : "No",
            red_bits, green_bits, blue_bits, alpha_bits, depth_bits);
}

static Bool WaitForMapNotify(Display *d, XEvent *e, char *arg) {
    return d && e && arg && (e->type == MapNotify) && (e->xmap.window == *(Window*) arg);
}

int main(int argc, char * argv[]) {
    bool drawXpm = False;
    int maxFps = 60;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--help")) printUsage();
        if (!strcmp(argv[i], "--xpm") && i == 1) drawXpm = True;
        else {
            maxFps = atoi(argv[i]);
            /*
                        printf("Unexpected argument `%s`\n", argv[i]);
                        printUsage();
             */
        }
    }


    display disp;
    newDisplay(&disp);
    window root;
    newRootWindow(&disp, &root);

    window goose;
    XSetWindowAttributes attr;

    int visAttr[] = {
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_DOUBLEBUFFER, True,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,
        GLX_DEPTH_SIZE, 24,
        None
    };

    int attr_mask, width, height;
    float speed = 3;
    int exitStatus = 0;
    float x = 0, y = 0, direction = speed;

    Framebuffer fb;
    initFB(&fb, disp, visAttr);


    attr.override_redirect = True;
    attr.colormap = fb.colorMap;
    attr.background_pixmap = None;
    attr.border_pixmap = None;
    attr.border_pixel = 0;
    attr.event_mask = StructureNotifyMask | EnterWindowMask | LeaveWindowMask | ExposureMask | ButtonPressMask | ButtonReleaseMask | OwnerGrabButtonMask | KeyPressMask | KeyReleaseMask;

    attr_mask = CWColormap | CWBorderPixel | CWEventMask | CWOverrideRedirect;

    width = DisplayWidth(disp.myDisplay, DefaultScreen(disp.myDisplay)) / 2;
    height = DisplayHeight(disp.myDisplay, DefaultScreen(disp.myDisplay)) / 2;
    x = width / 2, y = height / 2;

    createWindow(&disp, &goose, 0, 0, 400, 400, 0, fb.visual->depth, InputOutput, fb.visual->visual, attr_mask, &attr);
    initGLXWindow(&goose, &disp, fb);
    mapWindow(&disp, &goose);
    
    uint64_t startTime = getTimeMS();
    unsigned long inputTime = startTime;
    unsigned long newTime = 0;
    for (int i = 0; exitStatus == 0; i++) {
        int currentScrNum = getCurrentScreen(&disp, x, y);
        if (currentScrNum == 0)
            currentScrNum = 1;
        screen *scr = (screen *) L_getListElem(disp.List_myScreen, (currentScrNum - 1))->data;
        int currentScrHeight = scr->height;
        startTime = getTimeMS();

        redrawTheWindow(disp.myDisplay, goose.myGLX, 400, 400);
        XMoveWindow(disp.myDisplay, goose.myWindow, x, y);

        x += speed;
        y += direction;
        if ((y + 350) >= currentScrHeight) {
            direction = -speed;
        }
        if (y <= 0) {
            direction = speed;
        }
        flushDisplay(&disp);
        if (x > disp.disp_width) {
            x = -210;
        }

        exitStatus = inputLoop(&disp);
        unsigned long totalDrawTime = (newTime > startTime) ? newTime - startTime : startTime - newTime;
        if ((1000 / maxFps) > totalDrawTime) {
            msleep((1000 / maxFps) - totalDrawTime);
        }
        newTime = startTime;
    }
    destroyAllDisplayWindows(&disp);
    closeDisplay(&disp);
    return 0;
}
