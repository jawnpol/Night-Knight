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
#include <iostream>
#include <cmath>
using namespace std;

#define PI 3.141592653589793

extern bool structurePlacement(int x, int y);
extern struct timespec timeStart, timeCurrent;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);

//structure used to allow for mouse aiming
struct Aim {
    double x, y;
} m;

//Set of Structures for Unique Enemies
struct Zombie {
    float dir[2];
    float pos[2];
    float vel[2] = {0};
    float angle;
    float color[3];
    int wood = rand()%11, blood;
    bool dead, new_round = true;
    bool collision[16][9] = {false};

    void set_up()
    {
        if (!new_round)
            return;
        collision[16][9] = {false};
        dead = false;
        blood = 50;
        int num = rand()%2;
        if (num == 0) {
            pos[0] = rand()%1900+10;
            num = rand()%2;
            if  (num == 0)
                pos[1] = 10;
            else
                pos[1] = 1070;
        }
        else {
            pos[1] = rand()%1060+10;
            num = rand()%2;
            if (num == 0)
                pos[0] = 10;
            else
                pos[0] = 1910;
        }
        color[0] = 0.0f;
        color[1] = 0.5f;
        color[2] = 0.0f;
        angle = 0;
        new_round = false;
    }
}z[300];


struct Orc {
    int health;
    float dir[2];
    float pos[2];
    float vel[2];
    float angle;
    float color[3];
    int stone = rand()%11;
    bool dead, new_round = true;
    //Arrow
    float a_start[2];
    float a_pos[2];
    float a_vel[2] = {0};
    float a_angle;
    bool a_live = false;
    bool collision[16][9] = {false};

    void set_up()
    {
        if (!new_round)
            return;
        dead = false;
        health = 20;
        int num = rand()%2;
        if (num == 0) {
            pos[0] = rand()%1900+10;
            num = rand()%2;
            if  (num == 0)
                pos[1] = 10;
            else
                pos[1] = 1070;
        }
        else {
            pos[1] = rand()%1060+10;
            num = rand()%2;
            if (num == 0)
                pos[0] = 10;
            else
                pos[1] = 1910;
        }
        color[0] = 0.0;
        color[1] = 1.0;
        color[2] = 0.8;
        angle = 0.0f;
        new_round = false;
    }
}o[300];

struct Vampire {
    float dir[2];
    float pos[2];
    float vel[2];
    float angle;
    float color[3];
    int health = 50;
    int wood = rand()%21;
    int stone = rand()%21;
    bool dead, new_round = true;
    bool teleport = false;
    struct timespec time;

    void set_up() 
    {
        if (!new_round)
            return;
        teleport = false;
        dead = false;
        health = 50;
        int num = rand()%2;
        if (num == 0) {
            pos[0] = rand()%1900+10;
            num = rand()%2;
            if  (num == 0)
                pos[1] = 10;
            else
                pos[1] = 1070;
        }
        else {
            pos[1] = rand()%1060+10;
            num = rand()%2;
            if (num == 0)
                pos[0] = 10;
            else
                pos[1] = 1910;
        }
        color[0] = 1.0;
        color[1] = 0.5;
        color[2] = 0.0;
        angle = 0;
        new_round = false;
    }
}v[300];

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

bool structureCollision(int x, int y)
{
    for (int i = 0; i < 300; i++) {
        if (z[i].collision[x][y]) {
            z[i].collision[x][y] = false;
            return true;
        }
        if (o[i].collision[x][y]) {
            o[i].collision[x][y] = false;
            return true;
        }
    }
    return false;
}

void zw_z_pos(Zombie *z, int tX, int tY) 
{
    z->angle = atan2(z->pos[1]-tY, z->pos[0]-tX)*180/PI;
    int gridX, gridY;
    gridX = floor(z->pos[0]/120);
    gridY = floor(z->pos[1]/120);
    if (structurePlacement(gridX, gridY)) {
        z->vel[0] *= -1;
        z->vel[1] *= -1;
        z->collision[gridX][gridY] = true;
        return;
    }
    if (z->pos[0] > tX)
        z->vel[0] -= 0.001*tX;
    else if (z->pos[0] < tX)
        z->vel[0] += 0.001*tX;
    if (z->pos[1] > tY)
        z->vel[1] -= 0.001*tY;
    else if (z->pos[1] < tY)
        z->vel[1] += 0.001*tY;
}

void zw_o_pos(Orc *o, int tX, int tY) 
{
    float d0, d1;
    o->angle = atan2(o->pos[1]-tY, o->pos[0]-tX)*180/PI;
    if (o->a_live) {
        o->a_pos[0] += o->a_vel[0];
        o->a_pos[1] += o->a_vel[1];
        d0 = o->a_start[0] - o->a_pos[0];
        d1 = o->a_start[1] - o->a_pos[1];
        if (d0*d0+d1*d1 >= 100000)
            o->a_live = false;
        int gridX = floor(o->a_pos[0]/120);
        int gridY = floor(o->a_pos[1]/120);
        if (structurePlacement(gridX, gridY)) {
            o->a_live = false;
            o->collision[gridX][gridY] = true;
        }
    }
    d0 = o->pos[0]-tX;
    d1 = o->pos[1]-tY;
    if (d0*d0+d1*d1 <= 100000) {
        o->vel[0] = 0;
        o->vel[0] = 0;
        if (!o->a_live) {
            o->a_start[0] = o->pos[0];
            o->a_start[1] = o->pos[1];
            o->a_pos[0] = o->pos[0];
            o->a_pos[1] = o->pos[1];
            o->a_angle = o->angle;
            o->a_vel[0] = 5*sinf((-o->a_angle-90)*PI/180);
            o->a_vel[1] = 5*cosf((-o->a_angle-90)*PI/180);
            o->a_live = true;
        }
        return;
    }
    if (o->pos[0] > tX)
        o->vel[0] -= 0.001*tX;
    else if (o->pos[0] < tX)
        o->vel[0] += 0.001*tX;
    if (o->pos[1] > tY)
        o->vel[1] -= 0.001*tY;
    else if (o->pos[1] < tY)
        o->vel[1] += 0.001*tY;
}

void zw_v_pos(Vampire *v, int tX, int tY) 
{
    v->angle = atan2(v->pos[1]-tY, v->pos[0]-tX)*180/PI;
    if (v->teleport) {
        v->vel[0] = 0;
        v->vel[1] = 0;
        struct timespec tp;
        clock_gettime(CLOCK_REALTIME, &tp);
        double t = timeDiff(&v->time, &tp);
        if (t >= 1) {
            v->pos[0] = rand()%1900 + 10;
            v->pos[1] = rand()%1060 + 10;
            int gridX = v->pos[0]/120;
            int gridY = v->pos[1]/120;
            while (structurePlacement(gridX, gridY)) {
                v->pos[0] = rand()%1900 + 10;
                v->pos[1] = rand()%1060 + 10;
                gridX = v->pos[0]/120;
                gridY = v->pos[1]/120;
            }
            v->health += 10;
            v->teleport = false;
        }
        return;
    }
    if (v->pos[0] > tX)
        v->vel[0] -= 0.001*tX;
    else if (v->pos[0] < tX)
        v->vel[0] += 0.001*tX;
    if (v->pos[1] > tY)
        v->vel[1] -= 0.001*tY;
    else if (z->pos[1] < tY)
        v->vel[1] += 0.001*tY;
}

void zw_placeEnemies(GLuint t1, GLuint t2, GLuint t3, int round)
{
    for (int i = 0; i < round*2; i++) {
        if (z[i].dead)
            continue;
        glPushMatrix();
        glTranslatef(z[i].pos[0], z[i].pos[1], 0);
        glRotatef(z[i].angle-180, 0, 0, 1);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor4ub(255, 255, 255, 255);
        glBindTexture(GL_TEXTURE_2D, t1);
        glBegin(GL_QUADS);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-30,-25);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-30, 25);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(30, 25);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(30,-25);
        glEnd();
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_ALPHA_TEST);
    }
    if (round > 4) {
        for (int i = 0; i < (round-4)*2; i++) {
            if (o[i].dead)
                continue;
            glPushMatrix();
            glTranslatef(o[i].pos[0], o[i].pos[1], 0);
            glRotatef(o[i].angle-90, 0, 0, 1);
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0.0f);
            glColor4ub(255, 255, 255, 255);
            glBindTexture(GL_TEXTURE_2D, t2);
            glBegin(GL_QUADS);
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
            glTexCoord2f(0.0f, 1.0f); glVertex2i(-30,-25);
            glTexCoord2f(0.0f, 0.0f); glVertex2i(-30, 25);
            glTexCoord2f(1.0f, 0.0f); glVertex2i(30, 25);
            glTexCoord2f(1.0f, 1.0f); glVertex2i(30,-25);
            glEnd();
            glPopMatrix();
            glBindTexture(GL_TEXTURE_2D, 0);
            glDisable(GL_ALPHA_TEST);
        }
    }
    if (round > 9) {
        for (int i = 0; i < round-9; i++) {
            if (v[i].dead)
                continue;
            glPushMatrix();
            glTranslatef(v[i].pos[0], v[i].pos[1], 0);
            glRotatef(v[i].angle-90, 0, 0, 1);
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0.0f);
            glColor4ub(255, 255, 255, 255);
            glBindTexture(GL_TEXTURE_2D, t3);
            glBegin(GL_QUADS);
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
            glTexCoord2f(0.0f, 1.0f); glVertex2i(-30,-25);
            glTexCoord2f(0.0f, 0.0f); glVertex2i(-30, 25);
            glTexCoord2f(1.0f, 0.0f); glVertex2i(30, 25);
            glTexCoord2f(1.0f, 1.0f); glVertex2i(30,-25);
            glEnd();
            glPopMatrix();
            glBindTexture(GL_TEXTURE_2D, 0);
            glDisable(GL_ALPHA_TEST);
        }
    }
}

void zw_spawn_enemies(int round, int tX, int tY, GLuint t1, GLuint t2, GLuint t3)
{
    for (int i = 0; i < round*2; i++) {
        if (z[i].dead) {
            if (z[i].blood != 0) {
                glColor3f(1.0, 0.0, 0.0);
                glPushMatrix();
                glTranslatef(z[i].pos[0], z[i].pos[1], 0);
                glBegin(GL_LINES);
                for(int j = 0; j < 1000; j++) {
                    int r = rand()%100-rand()%50;
                    int r1 = rand()%100-rand()%50;
                    glVertex2f((float)r, (float)r1);
                }
                glEnd();
                glPopMatrix();
                z[i].blood--;
            }
            continue;
        }
        z[i].set_up();
        for (int k = i+1; k < round*2; k++) {
            if (z[k].dead)
                continue;
            float d0 = z[i].pos[0] - z[k].pos[0];
            float d1 = z[i].pos[1] - z[k].pos[1];
            if (d0*d0+d1*d1 <= 600) {
                z[i].vel[0] *= rand()%5-4;
                z[i].vel[1] *= rand()%5-4;
                z[i].pos[0] += z[i].vel[0];
                z[i].pos[1] += z[i].vel[1];
            }
        }
        for (int k = 0; k < (round-4)*2; k++) {
            if (o[k].dead || round < 5)
                continue;
            float d0 = z[i].pos[0] - v[k].pos[0];
            float d1 = z[i].pos[1] - v[k].pos[1];
            if (d0*d0+d1*d1 <= 600) {
                z[i].vel[0] *= rand()%5-4;
                z[i].vel[1] *= rand()%5-4;
                z[i].pos[0] += z[i].vel[0];
                z[i].pos[1] += z[i].vel[1];
            }
        }
        for (int k = 0; k < round-9; k++) {
            if (v[k].dead || round < 10)
                continue;
            float d0 = z[i].pos[0] - o[k].pos[0];
            float d1 = z[i].pos[1] - o[k].pos[1];
            if (d0*d0+d1*d1 <= 600) {
                z[i].vel[0] *= rand()%5-4;
                z[i].vel[1] *= rand()%5-4;
                z[i].pos[0] += z[i].vel[0];
                z[i].pos[1] += z[i].vel[1];
            }
        }
        zw_z_pos(&z[i], tX, tY);
        if (z[i].vel[0] >= 1)
            z[i].vel[0] = 1;
        if (z[i].vel[0] <= -1)
            z[i].vel[0] = -1;
        if (z[i].vel[1] >= 1)
            z[i].vel[1] = 1;
        if (z[i].vel[1] <= -1)
            z[i].vel[1] = -1;
        z[i].pos[0] += z[i].vel[0];
        z[i].pos[1] += z[i].vel[1];
        /*glColor3fv(z[i].color);
          glPushMatrix();
          glTranslatef(z[i].pos[0], z[i].pos[1], 0.0f);
          glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
          glBegin(GL_LINE_LOOP);
          for (float fill = 0; fill < 18; fill += 0.5) {
          for (int j = 0; j < 360; j++)
          glVertex2f(fill*sinf(j*3.14/180), fill*cosf(j*3.14/180));
          }
          glEnd();
          glPopMatrix();*/
    }
    if (round > 4) {
        for (int i = 0; i < (round-4)*2; i++) {
            if (o[i].dead)
                continue;
            o[i].set_up();
            int blood = 20 - o[i].health; 
            glColor3f(1.0, 0.0, 0.0);
            glPushMatrix();
            glTranslatef(o[i].pos[0], o[i].pos[1], 0);
            glBegin(GL_POINTS);
            for (int j = 0; j < blood*50; j++) {
                int r = rand()%50-rand()%25;
                int r1 = rand()%50-rand()%25;
                glVertex2f((float)r, (float)r1);
            }
            glEnd();
            glPopMatrix();
            if (o[i].a_live) {
                //draw arrow
                glPushMatrix();
                glTranslatef(o[i].a_pos[0], o[i].a_pos[1], 0.0f);
                glRotatef(o[i].a_angle+90, 0.0f, 0.0f, 1.0f);
                glColor3f(0.5, 0.5, 0.5);
                glBegin(GL_TRIANGLES);
                glVertex2f(0.0f,0.0f);
                glVertex2f(2.0f, -7.0f);
                glVertex2f(-2.0f, -7.0f);
                glEnd();
                glColor3f(0.5451, 0.2706, 0.0745);
                glBegin(GL_QUADS);
                glVertex2f(-1.0f, -7.0f);
                glVertex2f(1.0f, -7.0f);
                glVertex2f(1.0f, -30.0f);
                glVertex2f(-1.0f, -30.0f);
                glEnd();
                glColor3f(0.0, 0.3, 0.8);
                glBegin(GL_LINES);
                glVertex2f(-5.0f, -34.0f);
                glVertex2f(0.0f, -30.0f);
                glVertex2f(5.0f, -34.f);
                glVertex2f(0.0f, -30.0f);
                glVertex2f(-5.0f, -30.0f);
                glVertex2f(0.0f, -26.0f);
                glVertex2f(5.0f, -30.0f);
                glVertex2f(0.0f, -26.0f);
                glVertex2f(-5.0f, -26.0f);
                glVertex2f(0.0f, -22.0f);
                glVertex2f(5.0f, -26.0f);
                glVertex2f(0.0f, -22.0f);
                glEnd();
                glPopMatrix();
            }
            for (int k = i+1; k < (round-4)*2; k++) {
                if (o[k].dead)
                    continue;
                float d0 = o[i].pos[0] - o[k].pos[0];
                float d1 = o[i].pos[1] - o[k].pos[1];
                if (d0*d0+d1*d1 <= 600) {
                    o[i].vel[0] *= -1;
                    o[i].vel[1] *= -1;
                    o[i].pos[0] += o[i].vel[0];
                    o[i].pos[1] += o[i].vel[1];
                }
            }
            for (int k = 0; k < round-9; k++) {
                if (v[k].dead || round < 10)
                    continue;
                float d0 = o[i].pos[0] - v[k].pos[0];
                float d1 = o[i].pos[1] - v[k].pos[1];
                if (d0*d0+d1*d1 <= 600) {
                    o[i].vel[0] *= rand()%5-4;
                    o[i].vel[1] *= rand()%5-4;
                    o[i].pos[0] += o[i].vel[0];
                    o[i].pos[1] += o[i].vel[1];
                }
            }
            zw_o_pos(&o[i], tX, tY);
            if (o[i].vel[0] >= 1)
                o[i].vel[0] = 1;
            if (o[i].vel[0] <= -1)
                o[i].vel[0] = -1;
            if (o[i].vel[1] >= 1)
                o[i].vel[1] = 1;
            if (o[i].vel[1] <= -1)
                o[i].vel[1] = -1;
            o[i].pos[0] += o[i].vel[0];
            o[i].pos[1] += o[i].vel[1];
            /*glColor3fv(o[i].color);
              glPushMatrix();
              glTranslatef(o[i].pos[0], o[i].pos[1], 0.0f);
              glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
              glBegin(GL_LINE_LOOP);
              for (float fill = 0; fill < 18; fill += 0.5) {
              for (int j = 0; j < 360; j++)
              glVertex2f(fill*sinf(j*3.14/180), fill*cosf(j*3.14/180));
              }
              glEnd();
              glPopMatrix();*/
        }
    }
    if (round > 9) {
        for (int i = 0; i < round-9; i++) {
            if (v[i].dead)
                continue;
            v[i].set_up();
            if (v[i].teleport) {
                glPushMatrix();
                glTranslatef(v[i].pos[0], v[i].pos[1], 0.0f);
                glColor3f(1.0f, 1.0f, 1.0f);
                glBegin(GL_POLYGON);
                for (int line = 0; line < 360; line++) {
                    glVertex2f(60*sinf(line*3.14/180), 60*cosf(line*3.14/180));
                }
                glEnd();
                glPopMatrix();
            }
            int blood = 50 - v[i].health; 
            glColor3f(0.0, 0.0, 0.0);
            glPushMatrix();
            glTranslatef(v[i].pos[0], v[i].pos[1], 0);
            glBegin(GL_POINTS);
            for (int j = 0; j < blood*50; j++) {
                int r = rand()%50-rand()%25;
                int r1 = rand()%50-rand()%25;
                glVertex2f((float)r, (float)r1);
            }
            glEnd();
            glPopMatrix();
            for (int k = i+1; k < round-9; k++) {
                if (v[k].dead)
                    continue;
                float d0 = v[i].pos[0] - v[k].pos[0];
                float d1 = v[i].pos[1] - v[k].pos[1];
                if (d0*d0+d1*d1 <= 600) {
                    v[i].vel[0] *= rand()%5-4;
                    v[i].vel[1] *= rand()%5-4;
                    v[i].pos[0] += v[i].vel[0];
                    v[i].pos[1] += v[i].vel[1];
                }
            }
            zw_v_pos(&v[i], tX, tY);
            if (v[i].vel[0] >= 1)
                v[i].vel[0] = 1;
            if (v[i].vel[0] <= -1)
                v[i].vel[0] = -1;
            if (v[i].vel[1] >= 1)
                v[i].vel[1] = 1;
            if (v[i].vel[1] <= -1)
                v[i].vel[1] = -1;
            v[i].pos[0] += v[i].vel[0];
            v[i].pos[1] += v[i].vel[1];
            /*glColor3fv(v[i].color);
              glPushMatrix();
              glTranslatef(v[i].pos[0], v[i].pos[1], 0.0f);
              glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
              glBegin(GL_LINE_LOOP);
              for (float fill = 0; fill < 18; fill += 0.5) {
              for (int j = 0; j < 360; j++)
              glVertex2f(fill*sinf(j*3.14/180), fill*cosf(j*3.14/180));
              }
              glEnd();
              glPopMatrix();*/
        }
    }
    zw_placeEnemies(t1, t2, t3, round);
}

bool zw_check_enemy_hit(int round, float x, float y)
{
    for (int i = 0; i < round*2; i++) {
        if (z[i].dead) {
            continue;
        }
        float d0 = z[i].pos[0] - x;
        float d1 = z[i].pos[1] - y;
        if (d0*d0 + d1*d1 <= 324) {
            z[i].dead = true;
            return true;
        }
    }
    if (round > 4) {
        for (int i = 0; i < (round-4)*2; i++) {
            if (o[i].dead)
                continue;
            float d0 = o[i].pos[0] - x;
            float d1 = o[i].pos[1] - y;
            if (d0*d0 + d1*d1 <= 324) {
                o[i].health--;
                if (o[i].health == 0) {
                    o[i].dead = true;
                    return true;
                }
            }
        }
    }
    if (round > 9) {
        for (int i = 0; i < round-9; i++) {
            if (v[i].dead)
                continue;
            float d0 = v[i].pos[0] - x;
            float d1 = v[i].pos[1] - y;
            if (d0*d0 + d1*d1 <= 324) {
                v[i].health--;
                if (v[i].health == 0) {
                    v[i].dead = true;
                    return true;
                }
            }
        }
    }
    return false;
}


bool zw_player_hit(int round, float x, float y)
{
    for (int i = 0; i < round*2; i++) {
        if (z[i].dead)
            continue;
        float d0 = z[i].pos[0] - x;
        float d1 = z[i].pos[1] - y;
        if (d0*d0 + d1*d1 <= 400) { 
            z[i].vel[0] = -10.0;
            z[i].vel[1] = -10.0;
            z[i].pos[0] -= 0.01*x;
            z[i].pos[1] -= 0.01*y;
            return 1;
        }
    }
    if (round > 5) {
        for (int i = 0; i < (round-4)*2; i++) {
            if (!o[i].a_live)
                continue;
            float d0 = o[i].a_pos[0] - x;
            float d1 = o[i].a_pos[1] - y;
            if (d0*d0 + d1*d1 <= 400) {
                o[i].a_live = false; 
                return 1;
            }
        }
    }
    if (round > 9) {
        for (int i = 0; i < round-9; i++) {
            if (v[i].dead)
                continue;
            float d0 = v[i].pos[0] - x;
            float d1 = v[i].pos[1] - y;
            if (d0*d0 + d1*d1 <= 400 && !v[i].teleport) { 
                v[i].vel[0] = -10.0;
                v[i].vel[1] = -10.0;
                v[i].pos[0] -= 0.01*x;
                v[i].pos[1] -= 0.01*y;
                v[i].teleport = true;
                clock_gettime(CLOCK_REALTIME, &v[i].time);
                return 1;
            }
        }
    }
    return false;
}

void zw_reset_round()
{
    for (int i = 0; i < 300; i++) {
        z[i].new_round = true;
        z[i].set_up();
    }
    for (int i = 0; i < 300; i++) {
        o[i].new_round = true;
        o[i].set_up();
    }
    for (int i = 0; i < 300; i++) {
        v[i].new_round = true;
        v[i].set_up();
    }
}

void zw_drawSword(float x, float y, float angle)
{
    glPushMatrix();
    glColor3f(0.5, 0.5, 0.5);
    glTranslatef(x, y, 0.0f);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-2.0f, -7.0f);
    glVertex2f(2.0f, -7.0f);
    glVertex2f(2.0f, -30.0f);
    glVertex2f(-2.0f, -30.0f);
    glColor3f(0.5451, 0.2706, 0.0745);
    glVertex2f(-5.0f, -30.f);
    glVertex2f(5.0f, -30.f);
    glVertex2f(5.0f, -32.f);
    glVertex2f(-5.0f, -32.f);
    glVertex2f(-1.0f, -32.f);
    glVertex2f(1.0f, -32.f);
    glVertex2f(1.0f, -40.f);
    glVertex2f(-1.0f, -40.f);
    glEnd();
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f,0.0f);
    glVertex2f(2.0f, -7.0f);
    glVertex2f(-2.0f, -7.0f);
    glEnd();
    glColor3f(0.7, 0.7, 0.7);
    glBegin(GL_LINES);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.0f, -30.0f);
    glEnd();
    glPopMatrix();
}

bool zw_player_structure_collision(float x, float y)
{
    int gridX = floor(x/120);
    int gridY = floor(y/120);
    if (structurePlacement(gridX, gridY)) 
        return true;
    return false;
}
