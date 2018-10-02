#include "fonts.h"
#include <GL/glx.h>
#include <cstdlib>

void zw_show_credits(Rect &r, GLuint texid) {
    ggprint16(&r, 150, 0x0ffff00, "Zakary Worman");
    glPushMatrix();
    glColor3ub(255, 255, 255);
    glTranslatef(r.center+r.left/2 + rand()%10, r.center*1.5+rand()%10, 0);
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(0, 150);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(150, 150);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(150, 0);
    glEnd();
    glPopMatrix();
}
