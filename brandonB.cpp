//Author: Brandon Burdick
//Class:  CMPS 3350
//Group#: 9
//Game:   Night Knight:
//        Top Down 2D Survival Shooter Utilizing C++ and OpenGL
//	  
//Current Functions: Show_Credits -> outputs name in credit screen
//                   showPicture  -> prints picture in credit screen
//Future additions:  Powerups     -> create powerup to be used in NightKnight
//			***NOTES*** 
//			-Powerups currently spawn at random location
//			will change this to death location of asteroids soon.
//			-Also, need to implement collision detection of powerups
//			When this is done, powerups will be given specific 
//			attributes that affect character.
//                   Skins?       -> Different cosmetic looks for characters

#include<stdio.h>
#include<cstdlib>
#include"fonts.h"
#include<GL/glx.h>
#include<string.h>

#define MAXBUTTONS 4
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

struct Global {
	Button button[MAXBUTTONS];
	int numButtons;
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

void storeDeathPosition(float x, float y)
{
    d.x = x;
    d.y = y;
}

/*void spawnItem()
{
    if (spawnChance
}
*/
//function for determining the chances that powerup spawns
bool spawnChance(int chance) 
{
	int success = rand() % chance;
	if (success == 0) {
		return true;
	} else { 
		return false;
	}
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

        ggprint8b(&m, 16, 0x00ff0000, "Night-Knight");
        ggprint8b(&m, 16, 0x00ff0000, "Press space to play");

}

void menuScreenImage(int x, int y, GLuint texid)
{
    glColor3ub(255,255,255);
    int wid = 525;

    float fx = (float) x;
    float fy = (float) y;

    glPushMatrix();
    glTranslatef(fx/2,fy/2,0);
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid, -wid);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(wid, wid);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(wid, -wid);
    glEnd();
    glPopMatrix();

    

}

void initButtons() 
{
    glb.numButtons = 0;
    glb.button[glb.numButtons].btn.width = 140;
    glb.button[glb.numButtons].btn.height = 60;
    glb.button[glb.numButtons].btn.left = 20;
    glb.button[glb.numButtons].btn.bot = 320;
    glb.button[glb.numButtons].btn.right =
       glb.button[glb.numButtons].btn.left + glb.button[glb.numButtons].btn.width;
    glb.button[glb.numButtons].btn.top =
       glb.button[glb.numButtons].btn.bot + glb.button[glb.numButtons].btn.height;
    glb.button[glb.numButtons].btn.centerx =
       (glb.button[glb.numButtons].btn.left + glb.button[glb.numButtons].btn.right) / 2;
    glb.button[glb.numButtons].btn.centery =
       (glb.button[glb.numButtons].btn.bot + glb.button[glb.numButtons].btn.top) / 2;
    strcpy(glb.button[glb.numButtons].text, "Reset");
    glb.button[glb.numButtons].down = 0;
    glb.button[glb.numButtons].click = 0;
    glb.button[glb.numButtons].btnColor[0] = 1.0f;
    glb.button[glb.numButtons].btnColor[1] = 0.0f;
    glb.button[glb.numButtons].btnColor[2] = 0.0f;
    glb.button[glb.numButtons].dwnColor[0] = glb.button[glb.numButtons].btnColor[0] * 0.5f;
    glb.button[glb.numButtons].dwnColor[1] = glb.button[glb.numButtons].btnColor[1] * 0.5f;
    glb.button[glb.numButtons].dwnColor[2] = glb.button[glb.numButtons].btnColor[2] * 0.5f;
    glb.button[glb.numButtons].text_color = 0x00ffffff;
    glb.numButtons++;
}

void drawButtons()
{
    for (int i=0; i<glb.numButtons; i++) {
                if (glb.button[i].over) {
                        int w=2;
                        glColor3f(1.0f, 1.0f, 0.0f);
                        //draw a highlight around button
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
                /*r.left = glb.button[i].btn.centerx;
                r.bot  = glb.button[i].btn.centery-8;
                r.center = 1;
                if (glb.button[i].down) {
                        ggprint16(&r, 0, g.button[i].text_color, "Pressed!");
                } else {
                        ggprint16(&r, 0, g.button[i].text_color, g.button[i].text);
                }*/
        }
}

/*void startGame() 
{
    
}
*/
/*void mouseButtons()
{
    int i,x,y;
    int lbutton = 0;
    int rbutton = 0;
    for (i=0; i<glb.numButtons; i++) {
                glb.button[i].over=0;
                if (x >= glb.button[i].btn.left &&
                        x <= glb.button[i].btn.right &&
                        y >= glb.button[i].btn.bot &&
                        y <= glb.button[i].btn.top) {
                        glb.button[i].over=1;
                        if (glb.button[i].over) {
                                if (lbutton) {
                                        switch (i) {
                                                case 0:
                                                       // startGame();
                                                        break;
                                                case 1:
                                                        printf("Quit was clicked!\n");
                                                        return 1;
                                        }
                                }
                        }
                }
        }
}
*/
