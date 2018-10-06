//Finally a simple OpenAL example program.
//Gordon Griesel
//2015 - 2018
//Modified by: John Paul Cailing
//Main sound file
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <GL/glu.h>
#define USE_OPENAL_SOUND
#ifdef USE_OPENAL_SOUND
#include </usr/include/AL/alut.h>
#endif //USE_OPENAL_SOUND

static ALuint sound;
static ALuint alSource;

void initSound()
{
	//Get started right here.
    #ifdef USE_OPENAL_SOUND
	alutInit(0, NULL);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: alutInit()\n");
		return ;
	}
	//Clear error state.
	alGetError();
	//
	//Setup the listener.
	//Forward and up vectors are used.
	float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);
	

	//Buffer holds the sound information.
	sound  = alutCreateBufferFromFile("./fire.wav");
	//ALuint alSource;
	//Generate a source, and store it in a buffer.
	alGenSources(1, &alSource);
	alSourcei(alSource, AL_BUFFER , sound);
	//Set volume and pitch to normal, no looping of sound.
	alSourcef(alSource, AL_GAIN, 1.0f);
	alSourcef(alSource, AL_PITCH, 1.0f);
	alSourcei(alSource, AL_LOOPING, AL_FALSE);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		return ;
	}
//	alSourcePlay(alSource);

    //Play a looping sound
   /* alSourcef(alSource, AL_GAIN, 0.5f);
    alSourcef(alSource, AL_PITCH, 1.0f);
    alSourcei(alSource, AL_LOOPING, AL_TRUE);
    if (alGetError() != AL_NO_ERROR) {
        printf("Error: setting source\n");
        return ;
    }
    alSourcePlay(alSource);
    for (int i = 0; i < 42; i++) {
        alSourcePlay(alSource);
	//usleep(500000);
    }*/

	/*for (int i=0; i<4; i++) {
		alSourcePlay(alSource);
		usleep(250000);
	}*/
}

void cleanupSound()
{
    //Cleanup.
	//First delete the source.
	alDeleteSources(1, &alSource);
	//Delete the buffer.
	alDeleteBuffers(1, &sound);
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
    #endif //USE_OPENAL_SOUND
}

void playSound()
{
    #ifdef USE_OPENAL_SOUND
    alSourcePlay(alSource);
    #endif //USE_OPENAL_SOUND
}

