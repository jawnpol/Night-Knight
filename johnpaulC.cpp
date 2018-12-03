
//Author: John Paul Cailing
//Date: 09/25/2018
//Purpose: Contains credits and picture
//Main sound file for the project
//Contains building aspect
//In progress: 
// -Hovering over a graph square and placement from bship framework
//To do list:
// - Add sound for character movement
// - Multiple sounds for different weapons
// - Use an array to hold the buffers
// - Create a function to handle alSource 
// - Create materials class responsible for building
#include <stdio.h>
#include "fonts.h"
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <X11/Xutil.h>
#include <iostream>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
//#define USE_OPENAL_SOUND
//#ifdef USE_OPENAL_SOUND
#include </usr/include/AL/alut.h>
//#endif //USE_OPENAL_SOUND
using namespace std;

extern bool structureCollision(int x, int y);
extern bool menuScreen();

void buildReset(int x, int y);
void stoneReset(int x, int y);
bool matsCheck();
void showMaterials();
//void structureDamage();

void jc_show_credits(Rect &r)
{
	ggprint16(&r, 150, 0x00fff000, "John Paul Cailing");
}

void jpcShowPicture(int x, int y, GLuint texid)
{
	glColor3ub(255,255,255);
	int wid = 64;

	static float angle = 0.0f;
	float fx = (float) x;
	float fy = (float) y;

	fx += sin(angle) * 10.0f;
	angle += 0.2f;
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

//Declaring the buffer and source
static ALuint sound, song;
static ALuint alSource;
static ALuint gameSong;

void initSound()
{
	//Get started right here.
	//#ifdef USE_OPENAL_SOUND
	alutInit(0, NULL);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: alutInit()\n");
		return ;
	}
	//Clear error state.
	alGetError();

	//Setup the listener.
	//Forward and up vectors are used.
	float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);


	//Buffer holds the sound information.
	sound  = alutCreateBufferFromFile("./test.wav");
	song = alutCreateBufferFromFile("./cartoonsound.wav");

	//Generate a source, and store it in a buffer.
	alGenSources(1, &alSource);
	alGenSources(1, &gameSong);
	alSourcei(gameSong, AL_BUFFER, song);
	alSourcei(alSource, AL_BUFFER , sound);
	//Set volume and pitch to normal, no looping of sound.
	alSourcef(alSource, AL_GAIN, 1.0f);
	alSourcef(alSource, AL_PITCH, 1.0f);
	alSourcei(alSource, AL_LOOPING, AL_FALSE);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		return ;
	}

	//Play a looping sound
	//For future use as main game sound
	alSourcef(gameSong, AL_GAIN, 0.5f);
	alSourcef(gameSong, AL_PITCH, 1.0f);
	alSourcei(gameSong, AL_LOOPING, AL_TRUE);
	if (alGetError() != AL_NO_ERROR) {
		printf("Error: setting source\n");
		return ;
	}
}

void cleanupSound()
{
	//Cleanup.
	//First delete the source.
	alDeleteSources(1, &alSource);
	alDeleteSources(1, &gameSong);
	//Delete the buffer.
	alDeleteBuffers(1, &sound);
	alDeleteBuffers(1, &song);
	//Close out OpenAL itself.
	//Get active context.
	ALCcontext *Context = alcGetCurrentContext();
	//Get device for active context.
	ALCdevice *Device = alcGetContextsDevice(Context);
	//Disable context.
	alcMakeContextCurrent(NULL);
	//Release context(s).
	alcDestroyContext(Context);
	//Close device.
	alcCloseDevice(Device);
	//#endif //USE_OPENAL_SOUND
}

void playSound()
{
	//#ifdef USE_OPENAL_SOUND
	alSourcePlay(alSource);
	//#endif //USE_OPENAL_SOUND
}

void playGameSound()
{
	alSourcePlay(gameSong);
	/*for (int i = 0; i < 42; i++) {
	  cout << "Plays the song!";
	  alSourcePlay(gameSong);
	  usleep(1000000);
	  }*/
}


//Building functionality
//Shows grid on the game map
#define MAXGRID 16
//#define XDIM 16
//#define YDIM 9
const int XDIM = 16;
const int YDIM = 9;

bool menu = true; 

struct Grid {
	int over = 0;
	int status = 0;
	float color[4];
	bool woodStatus = false;
	bool stoneStatus = false;
}buildingGrid[16][9];

int currentRound;
int woodMats = 5;
int stoneMats = 10;
int woodCost = 10;
int stoneCost = 20;

struct Wood {
	float health = 100.0;
	float pos[2];
	bool dead = true;
}woodStore[XDIM][YDIM];

struct Stone {
	float health = 200.0;
	float pos[2];
	bool dead = true;
} stoneStore[XDIM][YDIM];

int gridDim = 120;
int quadSizei = 60;
int boardDimX = 1920;
int boardDimY = 1080;
int leftButton = 0;
int rightButton = 0;

void checkMouseEvent(int x, int y, int click)
{
	x = floor(x/120);
	y = floor(y/120);
	if (buildingGrid[x][y].status == 0 && click == 1 && woodMats >= woodCost) {
		buildingGrid[x][y].status = 1;
		buildingGrid[x][y].woodStatus = true;
		buildingGrid[x][y].stoneStatus = false;
		woodStore[x][y].dead = false;
		woodMats = woodMats - woodCost;
	}
	if (buildingGrid[x][y].status == 0 && click == 3 && stoneMats >= stoneCost) {
		buildingGrid[x][y].status = 1;
		buildingGrid[x][y].woodStatus = false;
		buildingGrid[x][y].stoneStatus = true;
		stoneStore[x][y].dead = false;
		stoneMats = stoneMats - stoneCost;
	}
	/*int x, y;
	  int i,j;
	  if (e->type == ButtonPress) {
	  if (e->xbutton.button == 1) {
	  if (roundEnd) {
	  leftButton = 1;
	//cout << "Button was pressed" << endl;
	}

	}
	if (e->xbutton.button == 3) {
	if (roundEnd) {
	rightButton = 1;
	//cout << "Right click" << endl;
	}
	}
	}

	for (i = 0; i < XDIM; i++) {
	for (j = 0; j < YDIM; j++) {
	buildingGrid[i][j].over = 0;
	//buildingGrid[i][j].status = 0;
	}
	}

	if (e->type == MotionNotify) {
	x = e->xbutton.x;
	y = e->xbutton.y;
	y = 1080 - y;
	for (i = 0; i < XDIM; i++) {
	for (j = 0; j < YDIM; j++) {
	if (roundEnd) {
	if (x <= i * gridDim + gridDim &&
	x >= i * gridDim - gridDim &&
	y <= j * gridDim  + gridDim &&
	y >= j * gridDim) {

	buildingGrid[i][j].over = 1;
	//buildingGrid[i][j].status = 0;  
	break;
	}
	}
	}
	if (buildingGrid[i][j].over == 1)
	break;
	}
	}

	if (roundEnd == 1 && leftButton == 1) {
	x = floor(x/120);
	y = floor(y/120);
	if (buildingGrid[x][y].status == 0) { 
	//if (leftButton == 1) {
	//cout << "Does it get to here?" << endl;
	if (woodMats >= woodCost) {
	buildingGrid[x][y].status = 1;
	buildingGrid[x][y].woodStatus = true;
	buildingGrid[x][y].stoneStatus = false;
	//woodStore[i][j].health = 100.0;
	woodStore[x][y].dead = false;
	cout << "Health during placement: " << woodStore[x][y].health;
	cout << "At: " << x << " " << y;
	cout << " Is it dead? " << woodStore[x][y].dead << endl;
	leftButton = 0;
	woodMats = woodMats - woodCost;
	}
	//cout << "i and j status 2" << buildingGrid[i][j].status << endl;
	}
	}

	if (roundEnd == 1 && rightButton == 1) {
	x = floor(x/120);
	y = floor(y/120);

	if (buildingGrid[x][y].status == 0) {
		if (stoneMats >= stoneCost) {
			buildingGrid[x][y].status = 1;
			buildingGrid[x][y].woodStatus = false;
			buildingGrid[x][y].stoneStatus = true;
			stoneStore[x][y].dead = false;
			rightButton = 0;
			stoneMats = stoneMats - stoneCost;
		}
	}
}*/	
}

void renderBoard(int xres, int yres)
{
	/*Rect r;
	  r.bot = boardDimY - 20;
	  r.left = boardDimX - 50;
	  r.center = boardDimX/2;
	  ggprint8b(&r, 16, 0x00000000, "Materials");
	  ggprint8b(&r, 16, 0x00000000, "Wood: %i", woodMats);
	  ggprint8b(&r, 16, 0x00000000, "Stone: %i", stoneMats);*/
	//cout << "Does it get into renderBoard?" << endl;
	int tileSize = 120;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glClear(GL_COLOR_BUFFER_BIT);
	//glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	for (int i = 0; i < xres; i += tileSize) {
		for (int j = 0; j < yres; j += tileSize) {
			//cout << "Does it get here"<< endl;
			glColor4f(1.0f, 1.0f, 1.0f, 0.5f);

			if (buildingGrid[i/120][j/120].over && matsCheck()) {
				glColor3f(0.3f, 1.0f, 0.3f);
				/*if (buildingGrid[i/120][j/120].woodStatus)
				  glBindTexture(GL_TEXTURE_2D, texture);
				  if (buildingGrid[i/tileSize][j/tileSize].stoneStatus)
				  glBindTexture(GL_TEXTURE_2D, stone);
				  */
			} 
			if (buildingGrid[i/120][j/120].over && !matsCheck())
				glColor3f(0.72f, 0.0f, 0.0f);

			glBindTexture(GL_TEXTURE_2D, 0);
			/*if (buildingGrid[i/120][j/120].status == 1) {
			  glBindTexture(GL_TEXTURE_2D, texture);
			  cout << "Does it get to the texture" << endl;
			  }*/
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2i(i,j);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(i,j + tileSize);
			glTexCoord2f(1.0f, 0.0f); glVertex2i(i + tileSize, j + tileSize);
			glTexCoord2f(1.0f, 1.0f); glVertex2i(i + tileSize, j);
			glEnd();
			//glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	glDisable(GL_BLEND);
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	for (int i = 0; i < xres; i += tileSize) {
		for (int j = 0; j < yres /*- 120*/; j += tileSize) {
			glVertex2i(i, j);
			glVertex2i(i, j + tileSize);
		}
	}
	for (int i = 0; i < xres; i += tileSize) {
		for (int j = 0; j < yres; j += tileSize) {
			glVertex2i(i, j);
			glVertex2i(i + tileSize , j);
		}
	}
	glEnd();  

}

//Building structures
//Adding a Materials class to handle building

void addWood(int wood)
{
	woodMats += wood;
}

void addStone(int stone)
{
	stoneMats += stone;
}

void renderHealth(int health)
{
	//int hSize = health * 15;
	glBegin(GL_QUADS);
	glColor3f(1, 0, 0);
	glVertex2f(0, 0);
	glVertex2f(50, 0);
	glVertex2f(50, health*50);
	glVertex2f(0, health*50);
	glEnd();
}

void buildPlacement(int xres, int yres, GLuint wood)
{
	int i, j;
	int tileSize = 120;

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
	for (i = 0; i < xres; i+=tileSize) {
		for (j = 0; j < yres; j+=tileSize) {
			if (buildingGrid[i/120][j/120].status == 1) {
				glBindTexture(GL_TEXTURE_2D, wood);
				glTexCoord2f(0.0f, 0.5f); glVertex2i(i, j);
				glTexCoord2f(0.0f, 0.0f); glVertex2i(i, j+tileSize);
				glTexCoord2f(0.5f, 0.0f); glVertex2i(i+tileSize, j+tileSize);
				glTexCoord2f(0.5f, 0.5f); glVertex2i(i+tileSize, j);
			}
		}
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
}

void gameBackground(int xres, int yres, GLuint texid, GLuint wood, GLuint stone)
{
	int tileSize = 120;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, texid);
	//glBegin(GL_QUADS);
	for (int i = 0; i < xres; i+=tileSize) {
		for (int j = 0; j < yres; j +=tileSize) {
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2i(i, j);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(i, j+tileSize);
			glTexCoord2f(1.0f, 0.0f); glVertex2i(i+tileSize, j+tileSize);
			glTexCoord2f(1.0f, 1.0f); glVertex2i(i+tileSize, j);
			glEnd();
			//glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	//structureRemoval();

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	//glBindTexture(GL_TEXTURE_2D, 0);

	for (int i = 0; i < xres; i+=tileSize) {
		for (int j = 0; j < yres; j +=tileSize) {
			if (woodStore[i/tileSize][j/tileSize].dead)
				continue;
			else 
				glBindTexture(GL_TEXTURE_2D, wood);
			//cout << "THIS BLOCK SHOULD BE DEAD: " << i/tileSize << " " << j/tileSize<< endl;
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2i(i + 30, j);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(i + 30, j+tileSize);
			glTexCoord2f(1.0f, 0.0f); glVertex2i(i+tileSize - 30, j+tileSize);
			glTexCoord2f(1.0f, 1.0f); glVertex2i(i+tileSize - 30, j);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	for (int i = 0; i < xres; i += tileSize) {
		for (int j = 0; j < yres; j += tileSize) {
			if (stoneStore[i/tileSize][j/tileSize].dead) {
				continue;
			} else {
				glBindTexture(GL_TEXTURE_2D, stone);
			}
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2i(i + 30, j);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(i + 30, j+tileSize);
			glTexCoord2f(1.0f, 0.0f); glVertex2i(i+tileSize - 30, j+tileSize);
			glTexCoord2f(1.0f, 1.0f); glVertex2i(i+tileSize - 30, j);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	glDisable(GL_ALPHA_TEST);
	//}
	}
void showMaterials()
{
	Rect r;
	r.bot = boardDimY - 28;
	r.left = boardDimX - 50;
	r.center = boardDimX - 20;
	ggprint8b(&r, 16, 0x00000000, "Materials");
	ggprint8b(&r, 16, 0x00000000, "Wood: %i", woodMats);
	ggprint8b(&r, 16, 0x00000000, "Stone: %i", stoneMats);

	Rect s;
	s.bot = boardDimY - 20;
	s.left = boardDimX - 50;
	s.center = boardDimX - 20;
	ggprint8b(&s, 16, 0xaaffaaaa, "Materials");
	ggprint8b(&s, 16, 0xaaffaaaa, "Wood: %i", woodMats);
	ggprint8b(&s, 16, 0xaaffaaaa, "Stone: %i", stoneMats);
}

void renderStructureHP()
{
	int health; 
	int x, y;
	for (int i = 0; i < XDIM; i++) {
		for (int j = 0; j < YDIM; j++) {
			if (!woodStore[i][j].dead) {
				health = woodStore[i][j].health / 5;
				x = i * 120;
				y = j * 120;
				glColor3f(0.0f, 0.0f, 0.0f);
				glBegin(GL_QUADS);
				glVertex2i(x + 50 - 1, y + 50 - 1);
				glVertex2i(x + 50 - 1, y + 55 + 1);
				glVertex2i(x + 50 + health + 1, y + 55 + 1);
				glVertex2i(x + 50 + health + 1, y + 50 - 1);
				glEnd();
				glColor3f(1.0f, 0.0f, 0.0f);
				glBegin(GL_QUADS);
				glVertex2i(x + 50, y + 50);
				glVertex2i(x + 50, y + 55);
				glVertex2i(x + 50 + health, y + 55);
				glVertex2i(x + 50 + health, y + 50);
				glEnd();

			}
			if (!stoneStore[i][j].dead) {

				health = stoneStore[i][j].health / 10;
				x = i * 120;
				y = j * 120;
				glColor3f(0.0f, 0.0f, 0.0f);
				glBegin(GL_QUADS);
				glVertex2i(x + 50 - 1, y + 50 - 1);
				glVertex2i(x + 50 - 1, y + 55 + 1);
				glVertex2i(x + 50 + health + 1, y + 55 + 1);
				glVertex2i(x + 50 + health + 1, y + 50 - 1);
				glEnd();

				glColor3f(1.0f, 0.0f, 0.0f);
				glBegin(GL_QUADS);
				glVertex2i(x + 50, y + 50);
				glVertex2i(x + 50, y + 55);
				glVertex2i(x + 50 + health, y + 55);
				glVertex2i(x + 50 + health, y + 50);
				glEnd();
			}
		}
	}
}



void playerModel(GLfloat pos[3], float angle, GLuint texture)
{	
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	glRotatef(angle-180, 0, 0, 1); 
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	glBindTexture(GL_TEXTURE_2D, texture);
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

bool structurePlacement(int x, int y)
{
	if (buildingGrid[x][y].status == 1)
	{
		cout << x << " " << y << endl;	
		return true;
	}
	return false;
}

void structureDamage()
{
	int i, j;
	for (i = 0; i < 16; i++) {
		for (j = 0; j < 9; j++) {
			if (structureCollision(i,j)) {
				cout << i << " " << j << " " << buildingGrid[i][j].status << endl;
				if (buildingGrid[i][j].woodStatus) {
					if (woodStore[i][j].health <= 0)
					{
						cout << i << " " << j << endl;
						buildingGrid[i][j].status = 0;
						cout << buildingGrid[i][j].status << endl;
						woodStore[i][j].dead = true;
						buildingGrid[i][j].woodStatus = false;
						return;
					}
					if (buildingGrid[i][j].status == 1) {
						cout << "Health: " << woodStore[i][j].health << endl;
						woodStore[i][j].health--;
					}
				}
				if (buildingGrid[i][j].stoneStatus) {
					if (stoneStore[i][j].health <= 0) {
						buildingGrid[i][j].status = 0;
						stoneStore[i][j].dead = true;
						buildingGrid[i][j].stoneStatus = false;
						return;
					}
					if (buildingGrid[i][j].status == 1) {
						stoneStore[i][j].health--;
					}
				}
			}
		}
	}
}

void buildReset()
{
	for (int i = 0; i < XDIM; i++) {
		for (int j = 0; j < YDIM; j++) {
			buildingGrid[i][j].woodStatus = false;
			buildingGrid[i][j].stoneStatus = false;
			buildingGrid[i][j].status = 0;
			woodStore[i][j].dead = true;
			stoneStore[i][j].dead = true;
		}
	}
	woodMats = 5;
	stoneMats = 10;
}

bool matsCheck()
{
	if (woodMats >= woodCost) {
		return true;
	}
	if (stoneMats >= stoneCost){
		return true;
	}
	return false;
}

void matsChange(int round)
{
	woodMats += ceil(round * 7.5);
	stoneMats += round * 10;
}
