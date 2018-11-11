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

//Function for determining the chances that powerup spawns
bool powerupChance(int chance) 
{
	int success = rand() % chance;
	if (success == 0) {
		return true;
	} else { 
		return false;
	}
}

//Function that spawns powerup at specified location
void spawnPowerup(int x_position, int y_position,int powerups[])
{
	//1 in 15 chance of spawning a powerup
	//Array element number correlates to different powerups
	if (powerupChance(15)) {
		powerups[0]=1;
		return;
	}
	if (powerupChance(15)) {
		powerups[1]=1;
		return;
	}
	if (powerupChance(15)) {
		powerups[2]=1;
		return;
	}
	if (powerupChance(15)) {
		powerups[3]=1;
		return;
	}
	if (powerupChance(15)) {
		powerups[4]=1;
		return;
	} else {
		return;
	}
}
