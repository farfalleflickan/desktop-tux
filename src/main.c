#include <X11/extensions/XInput2.h>
#include <X11/extensions/shape.h>
#include <X11/extensions/shapeconst.h>
#include <mpv/render_gl.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include "display.h"
#include "window.h"
#include "event.h"
#include "glxFB.h"
#include "utils.h"
#include "texture.h"

int printUsage() {
    printf("TBD\n");
    exit(0);
}

static void *get_proc_address_mpv(void *fn_ctx, const char *name) {
    return glXGetProcAddressARB((const GLubyte *) name);
}

int main(int argc, char * argv[]) {
    int maxFps = 60;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--help")) printUsage();
        else {
            maxFps = atoi(argv[i]);
        }
    }

    display disp;
    newDisplay(&disp);
    if (isCompositorRunning(disp.myDisplay, 0) != 1){
        fatalError("No compositor running!\n");
	}

	//Initialize SDL
	if( SDL_Init( SDL_INIT_AUDIO ) < 0 ){
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		exit(666);
	}
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0) {
		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        exit(666);
    }
	
	//The music that will be played
	Mix_Music *gMusic = NULL;
	Mix_Chunk *gBounce = NULL;
	gMusic = Mix_LoadMUS( "data/test.mp3" );
	if( gMusic == NULL ){
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        exit(666);
    }
	gBounce = Mix_LoadWAV("data/rebound.ogg");
	if( gBounce == NULL ){
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        exit(666);
    }

    window root, goose, myGIF;
    newRootWindow(&disp, &root);

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

    XSetWindowAttributes attr;
    int attr_mask;
    float speed = 5;
    int exitStatus = 0;
    float x = 0, y = 0, direction = speed;

    Framebuffer fb, gifFB;
    initFB(&fb, disp, visAttr);
    initFB(&gifFB, disp, visAttr);

    attr.override_redirect = True;
    attr.colormap = fb.colorMap;
    attr.background_pixmap = None;
    attr.border_pixmap = None;
    attr.border_pixel = 0;
    attr_mask = CWColormap | CWBorderPixel | CWOverrideRedirect; //CWColormap | CWBorderPixel | CWEventMask | CWOverrideRedirect; //attr.event_mask = StructureNotifyMask | EnterWindowMask | LeaveWindowMask | ExposureMask | ButtonPressMask | ButtonReleaseMask | OwnerGrabButtonMask | KeyPressMask | KeyReleaseMask;

    createWindow(&disp, &goose, 0, 0, disp.disp_width, disp.disp_height, 0, fb.visual->depth, InputOutput, fb.visual->visual, attr_mask, &attr);
    createWindow(&disp, &myGIF, 1500, 200, 128, 128, 0, gifFB.visual->depth, InputOutput, gifFB.visual->visual, attr_mask, &attr);

    initRenderContext(&goose, &disp, fb);
    mapWindow(&disp, &goose);

    texture Tux, pal;
    loadTexture(&Tux, "data/test.png");
    loadTexture(&pal, "data/palette.png");
    glViewport(0, 0, disp.disp_width, disp.disp_height);
    glOrtho(0, disp.disp_width, disp.disp_height, 0, -1, 1);
    // Makes input go through the window
    XserverRegion region = XFixesCreateRegion(disp.myDisplay, NULL, 0);
    XFixesSetWindowShapeRegion(disp.myDisplay, goose.myWindow, ShapeBounding, 0, 0, 0);
    XFixesSetWindowShapeRegion(disp.myDisplay, goose.myWindow, ShapeInput, 0, 0, region);
    XFixesDestroyRegion(disp.myDisplay, region);

    initRenderContext(&myGIF, &disp, gifFB);
    mapWindow(&disp, &myGIF);
    glXMakeCurrent(disp.myDisplay, myGIF.myWindow, myGIF.renderContext);
    mpv_handle *mpv = mpv_create();
    if (!mpv)
       fatalError("context init failed");

    mpv_set_option_string(mpv, "vo", "x11");
    mpv_set_option_string(mpv, "wid", myGIF.myXID);
    mpv_set_option_string(mpv, "loop-file", "inf");

    if (mpv_initialize(mpv) < 0)
       fatalError("mpv init failed");

    mpv_render_param params[3];
    params[0].type = MPV_RENDER_PARAM_API_TYPE;
    params[0].data = MPV_RENDER_API_TYPE_OPENGL;
    params[1].type = MPV_RENDER_PARAM_OPENGL_INIT_PARAMS;
    params[1].data = &(mpv_opengl_init_params){get_proc_address_mpv};
    params[2].type = MPV_RENDER_PARAM_ADVANCED_CONTROL;
    params[2].data = &(int){1};

    mpv_render_context *mpv_gl;
    if (mpv_render_context_create(&mpv_gl, mpv, params) < 0)
        fatalError("failed to initialize mpv GL context");

    const char *cmd[] = {"loadfile", "data/parrot.mp4", NULL};
    mpv_command_async(mpv, 0, cmd);
	
    uint64_t startTime = getTimeMS();
    unsigned long newTime = 0;
    while (exitStatus == 0) {
		if( Mix_PlayingMusic() == 0 ) {
			//Play the music
            Mix_PlayMusic( gMusic, -1 );
        }
        exitStatus = inputLoop(&disp);

        int i_x = x, i_y = y;
        int currentScrNum = getCurrentScreen(&disp, x, y);
        if (currentScrNum == 0) {
            currentScrNum = 1;
        }
        screen *scr = (screen *) L_getListElem(disp.List_myScreen, (currentScrNum - 1))->data;
        int currentScrHeight = scr->height;
        startTime = getTimeMS();

        XMoveWindow(disp.myDisplay, myGIF.myWindow, x, y);
        glXMakeCurrent(disp.myDisplay, myGIF.myWindow, myGIF.renderContext);

        params[0].type = MPV_RENDER_PARAM_OPENGL_FBO;
        params[0].data = &(mpv_opengl_fbo){0, myGIF.width, myGIF.height};
        params[1].type = MPV_RENDER_PARAM_FLIP_Y;
        params[1].data = &(int){1};
        params[2].type = 0;
        params[2].data = 0;
        mpv_render_context_update(mpv_gl);
        mpv_render_context_render(mpv_gl, params);
        glXSwapBuffers(disp.myDisplay, myGIF.myWindow);

		glXMakeCurrent(disp.myDisplay, goose.myWindow, goose.renderContext);
    	glClear(GL_COLOR_BUFFER_BIT);
        glPushMatrix();
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Disable antialiasing
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // 
		glBindTexture(GL_TEXTURE_2D, pal.myTexture);
	    glBegin(GL_QUADS);
    	glTexCoord2f(0, 0); glVertex2f(0, 0);
	    glTexCoord2f(1, 0); glVertex2f(200, 0);
    	glTexCoord2f(1, 1); glVertex2f(200, 200);
	    glTexCoord2f(0, 1); glVertex2f(0, 200);
    	glEnd();
	    glPopMatrix();
        renderTexture(&Tux, i_x, i_y);
		glXSwapBuffers(disp.myDisplay, goose.myWindow);
        flushDisplay(&disp);
        x += speed;
        y += direction;
        if ((y + Tux.height) >= currentScrHeight) {
            direction = -speed;
			Mix_PlayChannel( -1, gBounce, 0 );
        } else if (y <= 0) {
            direction = speed;
			Mix_PlayChannel( -1, gBounce, 0 );
        }
        if (x > disp.disp_width) {
            x = (int) -(Tux.width / 2);
        }

        unsigned long totalDrawTime = (newTime > startTime) ? newTime - startTime : startTime - newTime;
        if ((1000 / maxFps) > totalDrawTime) {
            msleep((1000 / maxFps) - totalDrawTime);
        }
        newTime = startTime;
    }
	Mix_FreeMusic(gMusic);
	Mix_FreeChunk(gBounce);
    destroyAllDisplayWindows(&disp);
    closeDisplay(&disp);
    return 0;
}
