#define ILUT_USE_OPENGL
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <stdlib.h>
#include <stdbool.h>
#include "string.h"
#include "texture.h"

static bool IL_READY = false;

void loadTexture(texture *tex, char* fileName) {
    if (!IL_READY) {
        ilInit();
        iluInit();
        ilutInit();
        IL_READY = true;
    }
    ILuint imgID = ilGenImage();
    ilBindImage(imgID);
    if (!ilLoadImage(fileName)) {
        ILenum error = ilGetError();
        if (error != IL_NO_ERROR) {
            do {
                printf("%s\n", iluErrorString(error));
            } while ((error = ilGetError()));
            exit(1);
        }
    }
    tex->filename = fileName;
    tex->tex_bpp = ilGetInteger(IL_IMAGE_BPP); // Image colour depth
    tex->width = ilGetInteger(IL_IMAGE_WIDTH); // Image width
    tex->height = ilGetInteger(IL_IMAGE_HEIGHT); // Image height
    tex->tex_format = ilGetInteger(IL_IMAGE_FORMAT); // Image format (i.e. RGB, RGBA, BGR etc.)

    ILenum error = ilGetError();
    if (error != IL_NO_ERROR) {
        do {
            printf("\n\n%s\n", iluErrorString(error));
        } while ((error = ilGetError()));
        exit(1);
    }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    ilutRenderer(ILUT_OPENGL);
    tex->myTexture = ilutGLBindTexImage();
    ilDeleteImages(1, &imgID);
}

void renderTexture(texture *tex, int x, int y) {
    glPushMatrix();
    glClear(GL_COLOR_BUFFER_BIT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glBindTexture(GL_TEXTURE_2D, tex->myTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(x, y + tex->height);
    glTexCoord2f(1, 0);
    glVertex2f(x + tex->width, y + tex->height);
    glTexCoord2f(1, 1);
    glVertex2f(x + tex->width, y);
    glTexCoord2f(0, 1);
    glVertex2f(x, y);
    glEnd();
    glPopMatrix();
}
