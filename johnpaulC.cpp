//Author: John Paul Cailing
//Date: 09/25/2018

#include <stdio.h>
#include "fonts.h"
#include <GL/glx.h>
#include "math.h"
void jc_show_credits(Rect &r, GLuint texid)
{
    ggprint16(&r, 150, 0x00fff000, "John Paul Cailing");
    glColor3ub(255,255,255);
    //int wid = 125;

    static float angle = 0.0f;
    float fx = (float) ((r.center + r.left/2));
    float fy = (float) (r.center* 1/2);

    fy += cos(angle) * 10.0f;
    fx += sin(angle) * 20.0f;
    angle += 0.2f;
    //glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslatef(fx,fy,0);
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(0,0);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(0, 125);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(125, 125);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(125, 0);
    glEnd();
    glPopMatrix();   
}

/*void jpcShowPicture(int x, int y, GLuint texid)
{
    glColor3ub(255,255,255);
    int wid = 64;

    static float angle = 0.0f;
    float fx = (float) x;
    float fy = (float) y;

    fx += sin(angle) * 10.0f;
    angle += 0.2f;
    //glColor3f(1.0, 1.0, 1.0);
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
}*/
