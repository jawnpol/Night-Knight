
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
void buildReset(int x, int y);
void stoneReset(int x, int y);
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

struct Grid {
	int over = 0;
	int status = 0;
	float color[4];
	bool woodStatus = false;
	bool stoneStatus = false;
}buildingGrid[16][9];

static int woodMats = 0;
static int stoneMats = 0;

struct Wood {
	float health = 100.0;
	float pos[2];
	bool dead = true;
}woodStore[XDIM][YDIM];

struct Stone {
	float health = 200.0;
	float pos[2];
	bool dead = false;
} stoneStore[XDIM][YDIM];

int gridDim = 120;
int quadSizei = 60;
int boardDimX = 1920;
int boardDimY = 1080;
int leftButton = 0;
int rightButton = 0;

/*void initBoard(void)
  {
  boardDimX = 1920;
  boardDimY = 1080;
  quadSize = 60;
  leftButton = 0;
  rightButton = 0;
  for (int i = 0; i < XDIM; i++)
  for (int j = 0; j< YDIM; j++) {
  buildingGrid[i][j].over = 0;
  buildingGrid[i][j].status = 0;
  }
  }*/
/*void buildingClick()
  {
  int i,j;
  for (i = 0; i < XDIM; i++) {
  for (j = 0; j < YDIM; j++) {
  if (leftButton)
  if (buildingGrid[i][j].over) {
  buildingGrid[i][j].status = 1;
  break;
  }
  cout << "Status at " << i << " && " << j << " == " << buildingGrid[i][j].status << endl;
  }
  if (buildingGrid[i][j].over && buildingGrid[i][j].status)
  break;
  }
  }*/

void checkMouseEvent(XEvent *e, bool roundEnd)
{
	int x, y;
	int i,j;
	int cent[2];

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
		/*for (i = 0; i < XDIM; i++) {
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
		}*/
	}

	if (roundEnd == 1 && leftButton == 1) {
		//cout << "Does it get here 1?" << endl;
		/*for (i = 0; i < XDIM; i++) {
		  for (j = 0; j < YDIM; j++) {
		  if (x <= i * gridDim + gridDim &&
		  x >= i * gridDim - gridDim &&
		  y <= j * gridDim + gridDim &&
		  y >= j * gridDim) {
		//cout << "i and j status" << buildingGrid[i][j].status << endl;
		if (buildingGrid[i][j].over == 1) { 
		//if (leftButton == 1) {
		//cout << "Does it get to here?" << endl;
		buildingGrid[i][j].status = 1;
		buildingGrid[i][j].woodStatus = true;
		buildingGrid[i][j].stoneStatus = false;
		//woodStore[i][j].health = 100.0;
		woodStore[i][j].dead = false;
		cout << "Health during placement: " << woodStore[i][j].health;
		cout << "At: " << i << " " << j;
		cout << " Is it dead? " << woodStore[i][j].dead << endl;
		leftButton = 0;
		//cout << "i and j status 2" << buildingGrid[i][j].status << endl;
		break;
		//}
		//leftButton = 0;
		}
		}
		}
		}*/
		x = floor(x/120);
		y = floor(y/120);
		if (buildingGrid[x][y].status == 0) { 
			//if (leftButton == 1) {
			//cout << "Does it get to here?" << endl;
			buildingGrid[x][y].status = 1;
			buildingGrid[x][y].woodStatus = true;
			buildingGrid[x][y].stoneStatus = false;
			//woodStore[i][j].health = 100.0;
			woodStore[x][y].dead = false;
			cout << "Health during placement: " << woodStore[x][y].health;
			cout << "At: " << x << " " << y;
			cout << " Is it dead? " << woodStore[x][y].dead << endl;
			leftButton = 0;
			//cout << "i and j status 2" << buildingGrid[i][j].status << endl;
		}

		/*if (roundEnd == 1 && rightButton == 1) {
			//cout << "Does it get here 1?" << endl;
			for (i = 0; i < XDIM; i++) {
				for (j = 0; j < YDIM; j++) {
					if (x <= i * gridDim + gridDim &&
							x >= i * gridDim - gridDim &&
							y <= j * gridDim + gridDim &&
							y >= j * gridDim) {
						//cout << "i and j status" << buildingGrid[i][j].status << endl;
						if (buildingGrid[i][j].over == 1) { 
							//if (leftButton == 1) {
							cout << "Does it get to here?" << endl;
							buildingGrid[i][j].status = 1;
							buildingGrid[i][j].stoneStatus = true;
							buildingGrid[i][j].woodStatus = false;
							stoneStore[i][j].dead = false;
							stoneStore[i][j].health = 200.0;
							rightButton = 0;
							cout << "i and j status 2" << buildingGrid[i][j].status << endl;
							break;
							//}
							//leftButton = 0;
						}
					}
				}
			}

		}*/
		}
	}

	/*bool menu;
	  void menuState(bool state)
	  {
	  menu = state;
	  }*/
	void renderBoard(int xres, int yres, GLuint texture, GLuint stone)
	{
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

				if (buildingGrid[i/120][j/120].over) {
					glColor3f(0.3f, 1.0f, 0.3f);
					/*if (buildingGrid[i/120][j/120].woodStatus)
					  glBindTexture(GL_TEXTURE_2D, texture);
					  if (buildingGrid[i/tileSize][j/tileSize].stoneStatus) {
					  glBindTexture(GL_TEXTURE_2D, stone);
					  cout << "It binds stone here!" << endl;
					  }*/
				}

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
				//glVertex2i(i + tileSize, j + tileSize);
				//glVertex2i(i + tileSize, j);
			}
		}
		for (int i = 0; i < xres; i += tileSize) {
			for (int j = 0; j < yres; j += tileSize) {
				glVertex2i(i, j);
				glVertex2i(i + tileSize , j);
				//glVertex2i(i + tileSize, j + tileSize);
			}
		}
		glEnd();  
		//}  
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

void gameBackground(int xres, int yres, GLuint texid, GLuint wood, GLuint stone, bool roundEnd)
{
	int tileSize = 120;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, texid);
	//glBegin(GL_QUADS);
	for (int i = 0; i < xres; i+=tileSize) {
		for (int j = 0; j < yres; j +=tileSize) {
			//cout << "Size of i: " << i << " and j : " << j << endl;
			//if(buildingGrid[i][j].status
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
			if (woodStore[i/tileSize][j/tileSize].dead //&&
					/*buildingGrid[i/tileSize][j/tileSize].status == 0*/) {
				//glBindTexture(GL_TEXTURE_2D, texid);
				continue;
			} else {
				glBindTexture(GL_TEXTURE_2D, wood);
				//cout << "THIS BLOCK SHOULD BE DEAD: " << i/tileSize << " " << j/tileSize<< endl;
			}


			//glBindTexture(GL_TEXTURE_2D, wood);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2i(i + 30, j);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(i + 30, j+tileSize);
			glTexCoord2f(1.0f, 0.0f); glVertex2i(i+tileSize - 30, j+tileSize);
			glTexCoord2f(1.0f, 1.0f); glVertex2i(i+tileSize - 30, j);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	/*for (int i = 0; i < xres; i += tileSize) {
	  for (int j = 0; j < yres; j += tileSize) {
	  if (buildingGrid[i/tileSize][j/tileSize].stoneStatus == false) 
	  continue;
	  glBindTexture(GL_TEXTURE_2D, stone);
	  glBegin(GL_QUADS);
	  glTexCoord2f(0.0f, 1.0f); glVertex2i(i + 30, j);
	  glTexCoord2f(0.0f, 0.0f); glVertex2i(i + 30, j+tileSize);
	  glTexCoord2f(1.0f, 0.0f); glVertex2i(i+tileSize - 30, j+tileSize);
	  glTexCoord2f(1.0f, 1.0f); glVertex2i(i+tileSize - 30, j);
	  glEnd();
	  glBindTexture(GL_TEXTURE_2D, 0);
	//stoneStore[i/tileSize][j/tileSize];
	stoneStore[i/tileSize][j/tileSize].pos[0] = i + 60;
	stoneStore[i/tileSize][j/tileSize].pos[1] = j + 60;
	}
	} */

	glDisable(GL_ALPHA_TEST);
	//}
	}


void playerModel(GLfloat color[], int colorSize, GLfloat pos[], int size, float angle, GLuint texture)
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
		//cout << "Checking build Status before return: " << buildingGrid[x][y].status << endl;
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
				if (woodStore[i][j].health <= 0)
				{
					cout << i << " " << j << endl;
					buildingGrid[i][j].status = 0;
					cout << buildingGrid[i][j].status << endl;
					woodStore[i][j].dead = true;
					return;
				}
				if (buildingGrid[i][j].status == 1) {
					cout << "Health: " << woodStore[i][j].health << endl;
					woodStore[i][j].health--;

				}

			}
		}
	}
	//cout << "Does it get to structureDamage" << endl;
	/*for (int i = 0; i < XDIM; i ++) {
	  for (int j = 0; j <YDIM; j++) {
	  cout << "Status before Collision: " << buildingGrid[i][j].status << endl;
	  while (structureCollision(i,j)) {
	  if (buildingGrid[i][j].status == 1) {
	  cout << "Original Health" << woodStore[i][j].health << endl;
	  woodStore[i][j].health = woodStore[i][j].health - 10.0;
	  cout << "Collision health: " << woodStore[i][j].health << endl;
	  cout << "Status in collision? " << buildingGrid[i][j].status << endl;
	  if (woodStore[i][j].health <=0.0) {
	  woodStore[i][j].dead = true;
	  buildingGrid[i][j].status = 0;
	  cout << "Is it dead?" << woodStore[i][j].dead << endl;
	  continue;
	  }
	//break;
	}
	}
	//if (woodStore[i][j].dead)
	//  buildReset(i,j);
	if (structureCollision(i,j)) {
	cout << "Does it get to check structureCollision" << endl;
	if (buildingGrid[i][j].woodStatus == true) {
	woodStore[i][j].health -= 60;
	cout << "Structure health:" << woodStore[i][j].health << endl;
	}
	if (buildingGrid[i][j].stoneStatus == true)
	stoneStore[i][j].health -= 1;
	}
	//}
	//}*/
}

void buildReset(int x, int y)
{
	buildingGrid[x][y].woodStatus = false;
	buildingGrid[x][y].stoneStatus = false;
	buildingGrid[x][y].status = 0;
	//woodStore[x][y].health = 100.0;
}

void stoneReset(int x, int y)
{
	//buildingGrid[x][y].woodStatus;
	//buildingGrid[x][y].stoneStatus;
	buildingGrid[x][y].status = 0;
	//stoneStore[x][y].health = 200.0;
}
