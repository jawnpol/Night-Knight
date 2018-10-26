//Author: John Paul Cailing
//Date: 09/25/2018
//Purpose: Contains credits and picture
//Main sound file for the project
//To do list:
// - Add sound for character movement
// - Multiple sounds for different weapons
// - Use an array to hold the buffers
// - Create a function to handle alSource 
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

enum {
    MODE_ROUND_START = 0,
    MODE_ROUND_END,
    MODE_BUILD,
    MODE_DEATH
};

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
//#define MAXGRID 16
//#define GRIDDIM 10

typedef struct t_grid {
	int displayState;
	int status;
	int over;
	float color[4];
} Grid;

int boardDim;
int gridDim = 40;

void initBoard(void)
{
	boardDim = 1920;
}

void renderBoard(int xres, int yres)
{
    //--------------------------------------------------------
    //This code is repeated several times in this program, so
    //it can be made more generic and cleaner with some work.
    int b2 = boardDim/2;
    int s0 = xres/2;
    int s1 = yres/2;
    //center of a grid
    //int cent[2];
    //bq is the width of one grid section
    //--------------------------------------------------------
    //glClearColor(0.0, 0.0, 0.0, 1.0);
    //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_BLEND);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    	glVertex2i(s0-b2, s1-b2);
        glVertex2i(s0-b2, s1+b2);
        glVertex2i(s0+b2, s1+b2);
        glVertex2i(s0+b2, s1-b2);
    glEnd();
    glDisable(GL_BLEND);
	
    int x0 = s0-b2;
    int x1 = s0+b2;
    int y0 = s1-b2;
    int y1 = s1+b2;
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_LINES);
    for (int i=1; i<gridDim; i++) {
        y0 += 100;
        glVertex2i(x0,y0);
        glVertex2i(x1,y0);
    }
    x0 = s0-b2;
    y0 = s1-b2;
    y1 = s1+b2;
    for (int j=1; j<gridDim; j++) {
        x0 += 100;
        glVertex2i(x0,y0);
        glVertex2i(x0,y1);
    }
    glEnd();
}
