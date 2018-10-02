//Author: Brandon Burdick
//Night-Knight source code altered

#include<stdio.h>
#include"fonts.h"
#include<GL/glx.h>

void bb_show_credits(Rect &r, GLuint texid)
{
    ggprint16(&r, 150, 0x00fff000, "Brandon Burdick");
    glColor3ub(255, 255, 255);
        glPushMatrix();
        glTranslatef(r.center + r.left/4, r.center/2, 0);
        glBindTexture(GL_TEXTURE_2D, texid);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
            glTexCoord2f(0.0f, 0.0f); glVertex2i(0, 150);
            glTexCoord2f(1.0f, 0.0f); glVertex2i(150, 150);
            glTexCoord2f(1.0f, 1.0f); glVertex2i(150, 0);
        glEnd();
        glPopMatrix();
}
	
