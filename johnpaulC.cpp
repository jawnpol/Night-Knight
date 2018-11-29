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
//#define USE_OPENAL_SOUND
//#ifdef USE_OPENAL_SOUND
#include </usr/include/AL/alut.h>
//#endif //USE_OPENAL_SOUND
using namespace std;
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
#define XDIM 16
#define YDIM 9

typedef struct t_grid {
    int over;
    int status = 0;
    float color[4];
} Grid;
Grid buildingGrid[XDIM][YDIM];
int gridDim = 120;
int quadSize;
int boardDimX;
int boardDimY;
int leftButton = 0;

void initBoard(void)
{
    boardDimX = 1920;
    boardDimY = 1080;
    quadSize = 60;
    leftButton = 0;
    for (int i = 0; i < XDIM; i++)
        for (int j = 0; j< YDIM; j++) {
            buildingGrid[i][j].over = 0;
            buildingGrid[i][j].status = 0;
        }

}
//Gets the center of each individual grid
//
void gridCenter(const int i, const int j, int cent[2])
{
    int boardX = boardDimX / 2;
    int boardY = boardDimY / 2;
    int screen_center[2] = {1920/2, 1080/2};
    
    int s0 = screen_center[0];
    int s1 = screen_center[1];

    int quad[2];

    int gridWidth = 120;
    
    quad[0] = s0 - boardX;
    quad[1] = s1 - boardY;
    cent[0] = quad[0] + gridWidth;
    cent[1] = quad[1] + gridWidth;
    cent[0] += (gridWidth * j);
    cent[1] += (gridWidth * i);
}


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
    x = e->xbutton.x;
    y = e->xbutton.y;
    y = 1080 - y;
    int i,j;
    int cent[2];
    
    if (e->type == ButtonPress) {
        if (e->xbutton.button == 1) {
            if (roundEnd) {
                leftButton = 1;
                cout << "Button was pressed" << endl;
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
        cout << "Does it get here 1?" << endl;
        for (i = 0; i < XDIM; i++) {
            for (j = 0; j < YDIM; j++) {
                if (x <= i * gridDim + gridDim &&
                    x >= i * gridDim - gridDim &&
                    y <= j * gridDim + gridDim &&
                    y >= j * gridDim) {
                    cout << "i and j status" << buildingGrid[i][j].status << endl;
                    if (buildingGrid[i][j].over == 1) { 
                        //if (leftButton == 1) {
                                cout << "Does it get to here?" << endl;
                                buildingGrid[i][j].status = 1;
                                leftButton = 0;
                                cout << "i and j status 2" << buildingGrid[i][j].status << endl;
                                break;
                        //}
                        //leftButton = 0;
                    }
                }
            }
        }
    }
}


/*bool menu;

void menuState(bool state)
{
    menu = state;
}*/
void renderBoard(int xres, int yres, GLuint texture)
{
    cout << "Does it get into renderBoard?" << endl;
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
                    glBindTexture(GL_TEXTURE_2D, texture);
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

static int woodMats = 0;
static int stoneMats = 0;
typedef struct t_wood {
    int health = 50;
} Wood;
Wood woodStore[XDIM][YDIM];

typedef struct t_stone {
    int health = 100;
} Stone;
Stone stoneStore[XDIM][YDIM];

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

void gameBackground(int xres, int yres, GLuint texid, GLuint wood, bool roundEnd)
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

    //if (roundEnd) {
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor4ub(255,255,255,255);
        glBindTexture(GL_TEXTURE_2D, 0);
        for (int i = 0; i < xres; i+=tileSize) {
            for (int j = 0; j < yres; j +=tileSize) {
            //cout << "Size of i: " << i << " and j : " << j << endl;
                //if(buildingGrid[i][j].status
                if (buildingGrid[i/tileSize][j/tileSize].status == 1) {
                    glBindTexture(GL_TEXTURE_2D, wood);
                    glBegin(GL_QUADS);
                        glTexCoord2f(0.0f, 1.0f); glVertex2i(i + 30, j);
                        glTexCoord2f(0.0f, 0.0f); glVertex2i(i + 30, j+tileSize);
                        glTexCoord2f(1.0f, 0.0f); glVertex2i(i+tileSize - 30, j+tileSize);
                        glTexCoord2f(1.0f, 1.0f); glVertex2i(i+tileSize - 30, j);
                    glEnd();
                    glBindTexture(GL_TEXTURE_2D, 0);
                }
            }
        }
        glDisable(GL_ALPHA_TEST);
    //}
}


void playerModel(GLfloat color[], int colorSize, GLfloat pos[], int size, float angle, GLuint texture)
{	
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_ONE, GL_ONE);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    glPushMatrix();
    //glColor3f(1.0, 1.0, 1.0);
    glTranslatef(pos[0], pos[1], pos[2]);
    glRotatef(angle-180, 0, 0, 1);
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
    	glTexCoord2f(0.0f, 1.0f); glVertex2i(-30,-25);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(-30, 25);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(30, 25);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(30,-25);
    glEnd();
    //glDisable(GL_BLEND);
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
}
