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
    int wood = rand()%11, blood;
    bool dead, new_round = true;

    void set_up() {
	if (!new_round)
	    return;
	dead = false;
	blood = 50;
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

    void set_up() {
	if (!new_round)
	    return;
	dead = false;
	health = 2;
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
    z->angle = atan2(z->pos[1]-tY, z->pos[0]-tX)*180/PI;
}

void zw_o_pos(Orc *o, int tX, int tY) {
    float d0, d1;
    o->angle = atan2(o->pos[1]-tY, o->pos[0]-tX)*180/PI;
    if (o->a_live) {
	o->a_pos[0] += o->a_vel[0];
	o->a_pos[1] += o->a_vel[1];
	d0 = o->a_start[0] - o->a_pos[0];
	d1 = o->a_start[1] - o->a_pos[1];
	if (d0*d0+d1*d1 >= 100000)
	    o->a_live = false;
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
		glColor3f(0.0, 0.4, 0.4);
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
	    //zw_v_pos(&v[i], tX, tY);
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
	    o[i].health--;
	    if (o[i].health == 0) {
	    	o[i].dead = true;
	    	return 1;
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
	if(d0*d0 + d1*d1 <= 400) { 
	    z[i].vel[0] = -10.0;
	    z[i].vel[1] = -10.0;
	    z[i].pos[0] -= 0.01*x;
	    z[i].pos[1] -= 0.01*y;
	    return 1;
	}
    }
    for (int i = 0; i < (round-4)*2; i++) {
	if (!o[i].a_live)
	    continue;
	float d0 = o[i].a_pos[0] - x;
	float d1 = o[i].a_pos[1] - y;
	if(d0*d0 + d1*d1 <= 400) {
	    o[i].a_live = false; 
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
