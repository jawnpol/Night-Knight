//Zakary Worman
//Used as an include file in our frame work to show my work progress
//My changes are intended to be towrds over all player movement and player physics for now
//Later changes by me will be towards creating enemies and their characteristics
//
//most of my changes/additions are here but some of them are documented in the asteroids.cpp program
//because it seemed that changing them their made the most sense
//all changes/additions I made in asteroids.cpp was prefaced with a comment noting as such

#include "fonts.h"
#include <GL/glx.h>
#include <cstdlib>
#include <cmath>

#define PI 3.141592653589793

//structure used to allow for mouse aiming
struct Aim {
    double x, y;
} m;

//this is used to save the mouse position from check mouse in main program
void zw_save_mouse_pos(int x, int y) {
    m.x = x;
    m.y = y;
}

//used to calculate the new angle of the ship based on the mouse
float zw_change_angle(double posx, double posy) {
    double xdiff = posx - m.x;
    double ydiff = posy - m.y;
    float angle = atan2(ydiff,xdiff)*180/PI+90.0f;
    return angle;
}

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
