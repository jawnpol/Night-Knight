//Author: Brandon Burdick
//Class:  CMPS 3350
//Group#: 9
//Game:   Night Knight:
//        Top Down 2D Survival Shooter Utilizing C++ and OpenGL
//	  
//My code includes:  
//-Powerups: Spawn on enemy death location
//-Hearts: Heal user on pickup. Spawn on enemy death location. 
//-Menu Screen: Button functionality and image rendering
//		*Button functionality modeled after Godon's snake framework*

#include<stdio.h>
#include<cstdlib>
#include"fonts.h"
#include<GL/glx.h>
#include<string.h>
#include<iostream>

using namespace std;

#define MAXBUTTONS 2
#define MAXHEARTS 5
typedef struct t_button {
	Rect btn;
	char text[32];
	int over;
	int down;
	int click;
	float btnColor[3];
	float dwnColor[3];
	unsigned int text_color;
} Button;

struct DeathPosition {
    float x, y;
} d;

struct Heart {
	float pos[2];
	float color[3];
	int value;
	bool spawn = false;
} h[5];

struct Global {
	Button button[MAXBUTTONS];
	int numButtons;
	int numHearts = 0;
} glb;

void bb_show_credits(Rect &r)
{
    ggprint16(&r, 150, 0x00fff000, "Brandon Burdick");
}

void bbShowPicture(int x, int y, GLuint texid)
{
    glColor3ub(255,255,255);
    int wid = 64;

    float fx = (float) x;
    float fy = (float) y;

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

//Could be used in the future for PowerUps implemented through image
//files. Need to fix whitespace issues behind image, for now powerups
//are just colored diamonds.
void renderPowerup(int x, int y, int red, int gre, int blu)
{
    glColor3ub(red,gre,blu);
	
    float fx = (float) x;
    float fy = (float) y;
	
    glPushMatrix();
    glTranslatef(fx,fy,0);
    glBegin(GL_TRIANGLES);
    glVertex2f(  0.0f,  16.0f);
    glVertex2f( 16.0f,   0.0f);
    glVertex2f(-16.0f,   0.0f);
    glVertex2f(-16.0f,   0.0f);
    glVertex2f( 16.0f,   0.0f);
    glVertex2f(  0.0f, -16.0f);
    glEnd();
    glPopMatrix();
}

void drawHeart()
{
	for (int i = 0; i < 5; i++) {
		if (h[i].spawn) {
			glColor3f(1.0, 0.0, 0.0);
			glPushMatrix();
			glTranslatef(h[i].pos[0], h[i].pos[1], 0);
			glBegin(GL_POLYGON);
			glVertex2i(0,0);
			glVertex2i(0,15);
			glVertex2i(7.5,22.5);
			glVertex2i(15,15);
			glVertex2i(15, 7.5);
			glVertex2i(0,-5);
			glVertex2i(-15,7.5);
			glVertex2i(-15,15);
			glVertex2i(-7.5,22.5);
			glVertex2i(0,15);
			glEnd();
			glPopMatrix();

			glColor3f(0.0, 0.0, 0.0);
			glPushMatrix();
			glTranslatef(h[i].pos[0],h[i].pos[1],0);
			glLineWidth(2);
			glBegin(GL_LINE_LOOP);
			glVertex2i(0,15);
			glVertex2i(7.5,22.5);
			glVertex2i(15,15);
			glVertex2i(15, 7.5);
			glVertex2i(0,-5);
			glVertex2i(-15,7.5);
			glVertex2i(-15,15);
			glVertex2i(-7.5,22.5);
			glVertex2i(0,15);
			glEnd();
			glPopMatrix();
		}
	}
}

void storeHeartPosition(float x, float y)
{
	for (int i = 0; i < 5; i++){
		if (!h[i].spawn) {
			h[i].pos[0] = x;
			h[i].pos[1] = y;
			h[i].spawn = true;
		}
	}
}
//function for determining the chances whether or not drops spawn
bool spawnChance(int chance) 
{
	int success = rand() % chance;
	if (success == 0) {
		return true;
	} else { 
		return false;
	}
}

void spawnHearts()
{
	for (int i=0; i < MAXHEARTS; i++) {
		if (h[i].spawn)
			continue;
		if (spawnChance(5)) {	
			h[i].spawn = true;
			h[i].pos[0] = d.x;
			h[i].pos[1] = d.y;
			break;
		}
	}
}

void storeDeathPosition(float x, float y)
{
	d.x = x;
	d.y = y;
	spawnHearts();
}


//function that spawns powerup at specified location
void powerupChance(int powerups[])
{
	//1 in 15 chance of spawning a powerup
	//array element number correlates to different powerups
	if (spawnChance(1)) {
		powerups[0]=1;
		return;
	}
	if (spawnChance(1)) {
		powerups[1]=1;
		return;
	}
	if (spawnChance(1)) {
		powerups[2]=1;
		return;
	}
	if (spawnChance(1)) {
		powerups[3]=1;
		return;
	}
	if (spawnChance(1)) {
		powerups[4]=1;
		return;
	} else {
		return;
	}
}

void spawnPowerups(int powerups[])
{
	for (int i=0;i<5;i++){
		if (powerups[i] == 1) {
			if (i==0) {
				renderPowerup(d.x,d.y,255,255,255);
			}
			if (i==1) {
				renderPowerup(d.x,d.y,255,0,0);
			}
			if (i==2) {
				renderPowerup(d.x,d.y,0,255,0);
			}
			if (i==3) {
				renderPowerup(d.x,d.y,0,0,255);
			}
			if (i==4) {
				renderPowerup(d.x,d.y,156,99,92);
			}
		}
	}
}

void printMenuScreen(float x, float y)
{
	//std::cout << "in menu screen\n";
	Rect m;
	m.bot = y - y/5;
	m.left = x/2;
	m.center = x/3;

	//weird error going on with below print. if removed button disappears?
	ggprint8b(&m, 16, 0x00ff0000, "");

}

void menuScreenImage(int x, int y, GLuint texid1, GLuint texid2)
{
	glColor3ub(255,255,255);

	glPushMatrix();
	glTranslatef((float)x/2,(float)y/2,0);
	glBindTexture(GL_TEXTURE_2D, texid1);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(-x/2, -y/2);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(-x/2, y/2);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(x/2, y/2);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(x/2, -y/2);
	glEnd();
	glPopMatrix();

	glColor3ub(255,255,255);

	int len = 348;//696
	int wid = 282;//564
	glPushMatrix();
	glTranslatef((float)x/4-160,(float)y/4+600,0);
	glBindTexture(GL_TEXTURE_2D, texid2);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(-len, -wid);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(-len, wid);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(len, wid);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(len, -wid);
	glEnd();
	glPopMatrix();



}

bool menuScreenState = true;
bool menuScreen()
{
	if (menuScreenState == true) {
		return true;
	} else {
		return false;
	}
}

void startGame() 
{
	menuScreenState = false;
}

//initializes position and shape of buttons
void initButtons() 
{
	glb.numButtons = 0;
	glb.button[glb.numButtons].btn.width = 240;  //240->Button Dimensions
	glb.button[glb.numButtons].btn.height = 120; //120->Saved in case
	glb.button[glb.numButtons].btn.left = 180;   //180->Changed in 
	glb.button[glb.numButtons].btn.bot = 400;    //400->Future
	glb.button[glb.numButtons].btn.right =
		glb.button[glb.numButtons].btn.left + glb.button[glb.numButtons].btn.width;
	glb.button[glb.numButtons].btn.top =
		glb.button[glb.numButtons].btn.bot + glb.button[glb.numButtons].btn.height;
	glb.button[glb.numButtons].btn.centerx =
		(glb.button[glb.numButtons].btn.left + glb.button[glb.numButtons].btn.right) / 2;
	glb.button[glb.numButtons].btn.centery =
		(glb.button[glb.numButtons].btn.bot + glb.button[glb.numButtons].btn.top) / 2;
	strcpy(glb.button[glb.numButtons].text, "Start Game");
	glb.button[glb.numButtons].down = 0;
	glb.button[glb.numButtons].click = 0;
	glb.button[glb.numButtons].btnColor[0] = 0.5f;
	glb.button[glb.numButtons].btnColor[1] = 0.5f;
	glb.button[glb.numButtons].btnColor[2] = 0.5f;
	glb.button[glb.numButtons].dwnColor[0] = 0.0f;
	glb.button[glb.numButtons].dwnColor[1] = 0.0f;
	glb.button[glb.numButtons].dwnColor[2] = 0.0f;
	glb.button[glb.numButtons].text_color = 0x00000000;
	glb.numButtons++;
}

//draws buttons and lines around them when mouse hovers/clicks
void drawButtons()
{
	Rect r;
	for (int i=0; i<glb.numButtons; i++) {
		if (glb.button[i].over==1) {
			int w=2;
			glColor3f(1.0f, 1.0f, 1.0f);
			//button highlight
			glLineWidth(3);
			glBegin(GL_LINE_LOOP);
			glVertex2i(glb.button[i].btn.left-w,  glb.button[i].btn.bot-w);
			glVertex2i(glb.button[i].btn.left-w,  glb.button[i].btn.top+w);
			glVertex2i(glb.button[i].btn.right+w, glb.button[i].btn.top+w);
			glVertex2i(glb.button[i].btn.right+w, glb.button[i].btn.bot-w);
			glVertex2i(glb.button[i].btn.left-w,  glb.button[i].btn.bot-w);
			glEnd();
			glLineWidth(1);
		}
		if (glb.button[i].down) {
			glColor3fv(glb.button[i].dwnColor);
		} else {
			glColor3fv(glb.button[i].btnColor);
		}
		glBegin(GL_QUADS);
		glVertex2i(glb.button[i].btn.left,  glb.button[i].btn.bot);
		glVertex2i(glb.button[i].btn.left,  glb.button[i].btn.top);
		glVertex2i(glb.button[i].btn.right, glb.button[i].btn.top);
		glVertex2i(glb.button[i].btn.right, glb.button[i].btn.bot);
		glEnd();
		r.left = glb.button[i].btn.centerx;
		r.bot  = glb.button[i].btn.centery-8;
		r.center = 1;
		ggprint16(&r, 0, glb.button[i].text_color, glb.button[i].text);
	}
}

void checkButtonClick(XEvent *e)
{
	int lclick = 0;
	int x,y;
	if (e->type == ButtonPress) {
		if (e->xbutton.button == 1) {
			lclick=1;
			//printf("Left click pressed\n");fflush(stdout);
		}
	}
	x = e->xbutton.x;
	y = e->xbutton.y;
	//y = (yres) - y
	y = 1080 - y;
	//printf("does this get called?%i %i", x, y);fflush(stdout);
	//if (e->type == ButtonPress) {
	//if (e->xbutton.button==1) {
	//lclick=1;
	for (int i=0; i < glb.numButtons; i++) {
		glb.button[i].over = 0;
		if (x >= glb.button[i].btn.left &&
				x <= glb.button[i].btn.right &&
				y >= glb.button[i].btn.bot &&
				y <= glb.button[i].btn.top) {
			glb.button[i].over=1;
			//printf("over button %i\n", i);fflush(stdout);
			if (glb.button[0].over==1) {
				if (lclick==1) {
					switch(i) {
						case 0:
							//printf("button clicked!\n");fflush(stdout);
							startGame();
							break;
					}
				}
			}
		}
	}

	if (e->type == MotionNotify) {
		if (x >= glb.button[0].btn.left &&
				x <= glb.button[0].btn.right &&
				y >= glb.button[0].btn.bot &&
				y <= glb.button[0].btn.top) {
			glb.button[0].over = 1;
		} else {
			glb.button[0].over = 0;
		}
	}
}
