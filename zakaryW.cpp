//Zakary Worman
//Used as an include file in our frame work to show my work progress
//My changes are intended to be towards over all player movement and player physics for now
//Later changes by me will be towards creating enemies and their characteristics
//
//most of my changes/additions are here but some of them are documented in the asteroids.cpp program
//because it seemed that changing them there made the most sense
//all changes/additions I made in asteroids.cpp were prefaced with a comment noting as such

#include "fonts.h"
#include <GL/glx.h>
#include <cstdlib>
#include <cmath>

#define PI 3.141592653589793

//structure used to allow for mouse aiming
struct Aim {
    double x, y;
} m;

//Set of Structures for Unique Enemies
struct Zombie {
    float dir[2];
    float pos[2];
    float vel[2];
    float angle;
    float color[3];
    int wood = rand()%11;

    Zombie() {
        int num = rand()%2;
        if (num == 0) {
            pos[0] = rand()%1920;
            num = rand()%2;
            if  (num == 0)
                pos[1] = 0;
            else
                pos[1] = 1080;
        }
        else {
           pos[1] = rand()%1080;
           num = rand()%2;
           if (num == 0)
               pos[0] = 0;
           else
               pos[1] = 1920;
        }
    }
};
struct Orc {
    float dir[2];
    float pos[2];
    float vel[2];
    float angle;
    float color[3];
    int stone = rand()%11;

    Orc() {
        int num = rand()%2;
        if (num == 0) {
            pos[0] = rand()%1920;
            num = rand()%2;
            if  (num == 0)
                pos[1] = 0;
            else
                pos[1] = 1080;
        }
        else {
           pos[1] = rand()%1080;
           num = rand()%2;
           if (num == 0)
               pos[0] = 0;
           else
               pos[1] = 1920;
        }
    }
};
struct Vampire {
    float dir[2];
    float pos[2];
    float vel[2];
    float angle;
    float color[3];
    int wood = rand()%21;
    int stone = rand()%21;

    Vampire() {
        int num = rand()%2;
        if (num == 0) {
            pos[0] = rand()%1920;
            num = rand()%2;
            if  (num == 0)
                pos[1] = 0;
            else
                pos[1] = 1080;
        }
        else {
           pos[1] = rand()%1080;
           num = rand()%2;
           if (num == 0)
               pos[0] = 0;
           else
               pos[1] = 1920;
        }
    }
};

//this is used to save the mouse position from check mouse in main program
void zw_save_mouse_pos(int x, int y)
{
    m.x = x;
    m.y = y;
}

//
void zw_gameover(double yres, double xres) 
{
        Rect n; 
        n.bot = yres/2;
        n.left = xres/2;
        n.center = xres/2;
        ggprint16(&n, 16, 0x00ff0000, "GAME OVER");
}

//used to calculate the new angle of the ship based on the mouse
float zw_change_angle(double posx, double posy)
{
    double xdiff = posx - m.x;
    double ydiff = posy - m.y;
    float angle = atan2(ydiff,xdiff)*180/PI+90.0f;
    return angle;
}

void zw_show_credits(Rect &r)
{
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
