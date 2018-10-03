//Zakary Worman
//Used as an include file in our frame work to show my work progress

#include "fonts.h"
#include <GL/glx.h>
#include <cstdlib>

void zw_show_credits(Rect &r) {
    ggprint16(&r, 150, 0x0ffff00, "Zakary Worman");
}

void zwShowPicture(int x, int y, GLuint texid)
{
    glColor3ub(255,255,255);
    int wid = 64;

    float fx = (float) x;
    float fy = (float) y;

    fx += rand()%10;
    fy += rand()%10;
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
