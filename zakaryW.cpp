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
    int wood = rand()%11;
    bool dead, new_round = true;

    void set_up() {
	if (!new_round)
	    return;
	dead = false;
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
		pos[0] = 1920;
	}
	color[0] = 0.0f;
	color[1] = 0.5f;
	color[2] = 0.0f;
	angle = 0;
	new_round = false;
    }
}z[300];
struct Orc {
    float dir[2];
    float pos[2];
    float vel[2];
    float angle;
    float color[3];
    int stone = rand()%11;
    bool dead, new_round = true;

    void set_up() {
	if (!new_round)
	    return;
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
	color[0] = 0.0;
	color[1] = 1.0;
	color[2] = 0.8;
	angle = 0.0f;
	new_round = false;
    }
}o[292];
struct Vampire {
    float dir[2];
    float pos[2];
    float vel[2];
    float angle;
    float color[3];
    int wood = rand()%21;
    int stone = rand()%21;
    bool dead, new_round = true;;

    void set_up() {
	if(!new_round)
	    return;
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
	color[0] = 1.0;
	color[1] = 0.5;
	color[2] = 0.0;
	angle = 0;
	new_round = false;
    }
}v[141];

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

void zw_z_pos(Zombie *z, int tX, int tY) {
    if(z->pos[0] > tX)
	z->vel[0] -= 0.001*tX;
    else if(z->pos[0] < tX)
	z->vel[0] += 0.001*tX;
    if(z->pos[1] > tY)
	z->vel[1] -= 0.001*tY;
    else if(z->pos[1] < tY)
	z->vel[1] += 0.001*tY;
}

void zw_o_pos(Orc *o, int tX, int tY) {
    if(o->pos[0] > tX)
	o->vel[0] -= 0.001*tX;
    else if(o->pos[0] < tX)
	o->vel[0] += 0.001*tX;
    if(o->pos[1] > tY)
	o->vel[1] -= 0.001*tY;
    else if(o->pos[1] < tY)
	o->vel[1] += 0.001*tY;
}

void zw_spawn_enemies(int round, int tX, int tY)
{
    for (int i = 0; i < round*2; i++) {
	if (z[i].dead)
	    continue;
	z[i].set_up();
	for (int k = i+1; k < round*2; k++) {
	    float d0 = z[i].pos[0] - z[k].pos[0];
	    float d1 = z[i].pos[1] - z[k].pos[1];
	    if (d0*d0+d1*d1 <= 600) {
		z[i].vel[0] *= -1;
		z[i].vel[1] *= -1;
	    }
	}
	zw_z_pos(&z[i], tX, tY);
	if(z[i].vel[0] >= 1)
	    z[i].vel[0] = 1;
	if(z[i].vel[0] <= -1)
	    z[i].vel[0] = -1;
	if(z[i].vel[1] >= 1)
	    z[i].vel[1] = 1;
	if(z[i].vel[1] <= -1)
	    z[i].vel[1] = -1;
	z[i].pos[0] += z[i].vel[0];
	z[i].pos[1] += z[i].vel[1];
	glColor3fv(z[i].color);
	glPushMatrix();
	glTranslatef(z[i].pos[0], z[i].pos[1], 0.0f);
	glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	for (float fill = 0; fill < 18; fill += 0.5) {
	    for (int j = 0; j < 360; j++)
		glVertex2f(fill*sinf(j*3.14/180), fill*cosf(j*3.14/180));
	}
	glEnd();
	glPopMatrix();
    }
    if (round > 4) {
	for (int i = 0; i < (round-4)*2; i++) {
	    if (o[i].dead)
		continue;
	    o[i].set_up();
	    for (int k = i+1; k < (round-4)*2; k++) {
		float d0 = o[i].pos[0] - o[k].pos[0];
		float d1 = o[i].pos[1] - o[k].pos[1];
		if (d0*d0+d1*d1 <= 600) {
		    o[i].vel[0] *= -1;
		    o[i].vel[1] *= -1;
		}
	    }
	    zw_o_pos(&o[i], tX, tY);
	    if(o[i].vel[0] >= 1)
		o[i].vel[0] = 1;
	    if(o[i].vel[0] <= -1)
		o[i].vel[0] = -1;
	    if(o[i].vel[1] >= 1)
		o[i].vel[1] = 1;
	    if(o[i].vel[1] <= -1)
		o[i].vel[1] = -1;
	    o[i].pos[0] += o[i].vel[0];
	    o[i].pos[1] += o[i].vel[1];
	    glColor3fv(o[i].color);
	    glPushMatrix();
	    glTranslatef(o[i].pos[0], o[i].pos[1], 0.0f);
	    glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
	    glBegin(GL_LINE_LOOP);
	    for (float fill = 0; fill < 18; fill += 0.5) {
		for (int j = 0; j < 360; j++)
		    glVertex2f(fill*sinf(j*3.14/180), fill*cosf(j*3.14/180));
	    }
	    glEnd();
	    glPopMatrix();
	}
	if (round > 10) {
	    for (int i = 0; i < round-9; i++) {
		if (v[i].dead)
		    continue;
		v[i].set_up();
		for (int k = i+1; k < round-9; k++) {
		    float d0 = v[i].pos[0] - v[k].pos[0];
		    float d1 = v[i].pos[1] - v[k].pos[1];
		    if (d0*d0+d1*d1 <= 600) {
			v[i].vel[0] *= -1;
			v[i].vel[1] *= -1;
		    }
		}
		zw_v_pos(&v[i], tX, tY);
		if(v[i].vel[0] >= 1)
		    v[i].vel[0] = 1;
		if(v[i].vel[0] <= -1)
		    v[i].vel[0] = -1;
		if(v[i].vel[1] >= 1)
		    v[i].vel[1] = 1;
		if(v[i].vel[1] <= -1)
		    v[i].vel[1] = -1;
		v[i].pos[0] += v[i].vel[0];
		v[i].pos[1] += v[i].vel[1];
		glColor3fv(v[i].color);
		glPushMatrix();
		glTranslatef(v[i].pos[0], v[i].pos[1], 0.0f);
		glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
		glBegin(GL_LINE_LOOP);
		for (float fill = 0; fill < 18; fill += 0.5) {
		    for (int j = 0; j < 360; j++)
			glVertex2f(fill*sinf(j*3.14/180), fill*cosf(j*3.14/180));
		}
		glEnd();
		glPopMatrix();
	    }
	}
    }
}

bool zw_check_enemy_hit(int round, float x, float y)
{
    for (int i = 0; i < round*2; i++) {
	if (z[i].dead)
	    continue;
	float d0 = z[i].pos[0] - x;
	float d1 = z[i].pos[1] - y;
	if(d0*d0 + d1*d1 <= 324) {
	    z[i].dead = true;
	    return 1;
	}
    }
    for (int i = 0; i < round*2; i++) {
	if (o[i].dead)
	    continue;
	float d0 = o[i].pos[0] - x;
	float d1 = o[i].pos[1] - y;
	if(d0*d0 + d1*d1 <= 324) {
	    o[i].dead = true;
	    return 1;
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
	if(d0*d0 + d1*d1 <= 400) { 
	    z[i].vel[0] *= -1.0;
	    z[i].vel[1] *= -1.0;
	    return 1;
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
    for (int i = 0; i < 292; i++) {
	o[i].new_round = true;
	o[i].set_up();
    }
    for (int i = 0; i < 141; i++) {
	v[i].new_round = true;
	v[i].set_up();
    }
}
