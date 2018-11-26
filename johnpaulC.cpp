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
//#define USE_OPENAL_SOUND
//#ifdef USE_OPENAL_SOUND
#include </usr/include/AL/alut.h>
//#endif //USE_OPENAL_SOUND

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
	//song = alutCreateBufferFromFile("./cartoonsound.wav");

	//Generate a source, and store it in a buffer.
	alGenSources(1, &alSource);
	//alGenSources(1, &gameSong);
	//alSourcei(gameSong, AL_BUFFER, song);
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
	/*alSourcef(gameSong, AL_GAIN, 0.5f);
	alSourcef(gameSong, AL_PITCH, 1.0f);
	alSourcei(gameSong, AL_LOOPING, AL_TRUE);
	if (alGetError() != AL_NO_ERROR) {
		printf("Error: setting source\n");
		return ;
	}*/
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

/*void playGameSound()
{
	#ifdef USE_OPENAL_SOUND
	for (int i = 0; i < 42; i++) {
		alSourcePlay(gameSong);
		usleep(54100000);
	}
	#endif //USE_OPENAL_SOUND
}*/


//Building functionality
//Shows grid on the game map
#define MAXGRID 16

typedef struct t_grid {
	int displayState;
	int status;
	int over;
	float color[4];
} Grid;
Grid buildingGrid[MAXGRID][MAXGRID];

int gridDim = 120;
int boardDim;
int quadSize;

void initBoard(void)
{
	boardDim = 1920;
    quadSize = (boardDim / gridDim) / 2 - 1;
}

void renderBoard(int xres, int yres)
{
    int boardSize = boardDim/2;
    int xBorder = xres/2;
    int yBorder = yres/2;
    
    //glClearColor(0.0, 0.0, 0.0, 1.0);
    //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_BLEND);
    /*glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    	glVertex2i(xBorder-boardSize, yBorder-boardSize);
        glVertex2i(xBorder-boardSize, yBorder+boardSize);
        glVertex2i(xBorder+boardSize, yBorder+boardSize);
        glVertex2i(xBorder+boardSize, yBorder-boardSize);
    glEnd();*/
    //glDisable(GL_BLEND);
	
    int x0 = xBorder-boardSize;
    int x1 = xBorder+boardSize;
    int y0 = yBorder-boardSize;
    int y1 = yBorder+boardSize;
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_LINES);
    for (int i=1; i<gridDim; i++) {
        y0 += gridDim;
        glVertex2i(x0,y0);
        glVertex2i(x1,y0);
    }
    x0 = xBorder-boardSize;
    y0 = yBorder-boardSize;
    y1 = yBorder+boardSize;
    for (int j=1; j<gridDim; j++) {
        x0 += gridDim;
        glVertex2i(x0,y0);
        glVertex2i(x0,y1);
    }
    glEnd();
}

void gridCenter(int xres, int yres, const int i, const int j, int cent[2])
{
    int quad[2];
    int xBorder = xres / 2;
    int yBorder = yres / 2;

    quad[0] = xBorder - (boardDim / 2);
    quad[1] = yBorder - (boardDim / 2);
    cent[0] = quad[0] + ((boardDim / gridDim) / 2);
    cent[1] = quad[1] + ((boardDim / gridDim) / 2);
    cent[0] += (boardDim / gridDim) * j;
    cent[1] += (boardDim / gridDim) * i;

}

//Building structures
//Adding a Materials class to handle building
typedef struct t_wood {
    int health = 50;
    int repair = 30;
    double buildTime = 1.5;
} Wood;

typedef struct t_stone {
    int health = 150;
    int repair = 100;
    int damageResistance = 10;
    double buildTime = 2.5;
} Stone;

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

void gameBackground(int xres, int yres, GLuint texid)
{
    int tileSize = 100;
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);
    for (int i = 0; i < xres; i+=tileSize) {
        for (int j = 0; j < yres; j +=tileSize) {
                glTexCoord2f(0.0f, 1.0f); glVertex2i(i, j);
                glTexCoord2f(0.0f, 0.0f); glVertex2i(i, j+tileSize);
                glTexCoord2f(1.0f, 0.0f); glVertex2i(i+tileSize, j+tileSize);
                glTexCoord2f(1.0f, 1.0f); glVertex2i(i+tileSize, j);
        }
    }
    glEnd();
    //glBindTexture(GL_TEXTURE_2D, 0);

}

void playerModel(GLfloat color[], int colorSize, GLfloat pos[], int size, GLfloat angle, GLuint texture)
{	
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_ONE, GL_ONE);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    glPushMatrix();
    //glColor3f(1.0, 1.0, 1.0);
    glTranslatef(pos[0], pos[1], pos[2]);
    //glBindTexture(GL_TEXTURE_2D, texture);
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV, GL_REPLACE);
    //glBindTexture(GL_TEXTURE_2D, texture);    
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    //glTranslatef(pos[0], pos[1], pos[2]);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    	glTexCoord2f(0.0f, 1.0f); glVertex2i(-30,-40);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(-30, 40);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(30, 40);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(30,-40);
    glEnd();
    //glDisable(GL_BLEND);
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
}

/*void playerModel(GLfloat color[], int colorSize, GLfloat pos[], int size, GLfloat angle, GLuint texture)
{
    float *c = color;
    //glColor3fv(c);i
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glColor3f(0.0f, 0.0f, 0.0f);
    //glDisable(GL_TEXTURE_2D);
    //glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, texture);
    //glPushMatrix();
    //glTranslatef(pos[0], pos[1], pos[2]);
    
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, texture);
    //This is the drawing ofthe item
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-30,-40);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-30, 40);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(30, 40);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(30,-40);
    glEnd();
    glPopMatrix();
}*/

