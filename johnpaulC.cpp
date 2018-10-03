//Author: John Paul Cailing
//Date: 09/25/2018

#include <stdio.h>
#include "fonts.h"
#include <GL/glx.h>
#include "math.h"
void jc_show_credits(Rect &r)
{
    ggprint16(&r, 150, 0x00fff000, "John Paul Cailing");
}

void jpcShowPicture(int x, int y, GLuint texid)
{
    glColor3ub(255,255,255);
    int wid = 64;

    static float angle = 0.0f;
    float fx = (float) x;
    float fy = (float) y;

    fx += sin(angle) * 10.0f;
    angle += 0.2f;
    glPushMatrix();
    glTranslatef(fx,fy,0);
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid, -wid);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(wid, wid);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(wid, -wid);
    glEnd();
    glPopMatrix();
}
