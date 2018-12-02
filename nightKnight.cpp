//
//
//
//
//program: asteroids.cpp
//author:  Gordon Griesel
//date:    2014 - 2018
//mod spring 2015: added constructors
//mod spring 2018: X11 wrapper class
//This program is a game starting point for a 3350 project.
//
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cmath>
using namespace std;
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "fonts.h"

//defined types
typedef float Flt;
typedef float Vec[3];
typedef Flt Matrix[4][4];

//macros
#define rnd() (((Flt)rand())/(Flt)RAND_MAX)
#define random(a) (rand()%(a))
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
			     (c)[1]=(a)[1]-(b)[1]; \
(c)[2]=(a)[2]-(b)[2]
//constants
const float TIMESLICE = 1.0f;
const float GRAVITY = -0.2f;
#define PI 3.141592653589793
#define ALPHA 1
const int MAX_BULLETS = 100;
const Flt MINIMUM_ASTEROID_SIZE = 60.0;
int powerups[5] = {0,0,0,0,0};
extern void initSound();
extern void cleanupSound();
extern void playSound();
extern void playGameSound();
//-----------------------------------------------------------------------------
//Setup timers
const double OOBILLION = 1.0 / 1e9;
extern struct timespec timeStart, timeCurrent;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);
//-----------------------------------------------------------------------------
//Group extern includes to use personal files
extern void zw_show_credits(Rect &r);
extern void zk_show_credits(Rect &r);
extern void zk_gameoverimage(int x, int y, GLuint texid);
extern void zk_gameovertext(int x, int y);
extern void zk_showhealthtext(int x, int y);
extern void zk_savemouse(int x, int y);
extern void zk_resetpups(int powerups[]);
extern void zk_blackbar();
extern void zk_pausetext(int x, int y);
extern void zk_pausemenu(int x, int y);
extern void zk_drawCircle();
extern void bbShowPicture(int x, int y, GLuint texid);
extern void bb_show_credits(Rect &r);
extern void jc_show_credits(Rect &r);
extern void zwShowPicture(int x, int y, GLuint texid);
extern void zkShowPicture(int x, int y, GLuint texid);
extern bool spawnChance(int chance);
extern void spawnHearts();
extern void drawHeart();
extern bool heartCollision(float x, float y);
extern bool powerupCollision(float x, float y);
extern bool checkSpeed();
extern bool checkFireRate();
extern void resetPowerups();
extern void spawnPowerups(int powerups[]);
extern void printMenuScreen(float x, float y);
extern void storeDeathPosition(float x, float y);
extern bool menuScreen();
extern bool creditsScreen();
extern void closeCredits();
extern void openCredits();
extern void startGame();
extern void initButtons();
extern void drawButtons();
extern void drawPowerups();
extern void checkButtonClick(XEvent *e);
extern void menuScreenImage(int x, int y, GLuint texid1, GLuint texid2);
extern void spawnPowerup(int x_position, int y_position);
extern void jpcShowPicture(int x, int y, GLuint texid);
extern void zw_save_mouse_pos(int x, int y);
extern float zw_change_angle(double posx, double posy);
extern void zw_gameover(double yres, double xres);
extern void zw_spawn_enemies(int round, int tX, int tY, GLuint t1, GLuint t2, GLuint t3);
extern bool zw_check_enemy_hit(int round, float x, float y);
extern bool zw_player_hit(int round, float x, float y);
extern void playerModel(GLfloat color[],int cSize, GLfloat pos[], int size, float angle, GLuint texture);
extern void gameBackground(int xres, int yres, GLuint texid, GLuint woodTexture, GLuint stoneTexture, bool roundEnd);
extern void renderHealth(int health);
extern void renderBoard(int xres, int yres, GLuint texture, GLuint stone);
extern void buildPlacement(int xres, int yres, GLuint wood);
extern void structureDamage();
//extern void structureRemoval();
//extern void menuState(bool state);
extern void checkMouseEvent(XEvent *e, bool roundEnd);
extern void zw_reset_round();
extern void zw_drawSword(float x, float y, float angle);
extern bool zw_player_structure_collision(float x, float y);
//-----------------------------------------------------------------------------
class Image {
	public:
		int width, height;
		unsigned char *data;
		~Image() { delete [] data; }
		Image(const char *fname) {
			if (fname[0] == '\0')
				return;
			//printf("fname **%s**\n", fname);
			int ppmFlag = 0;
			char name[40];
			strcpy(name, fname);
			int slen = strlen(name);
			char ppmname[80];
			if (strncmp(name+(slen-4), ".ppm", 4) == 0)
				ppmFlag = 1;
			if (ppmFlag) {
				strcpy(ppmname, name);
			} else {
				name[slen-4] = '\0';
				//printf("name **%s**\n", name);
				sprintf(ppmname,"%s.ppm", name);
				//printf("ppmname **%s**\n", ppmname);
				char ts[100];
				//system("convert eball.jpg eball.ppm");
				sprintf(ts, "convert %s %s", fname, ppmname);
				system(ts);
			}
			//sprintf(ts, "%s", name);
			FILE *fpi = fopen(ppmname, "r");
			if (fpi) {
				char line[200];
				fgets(line, 200, fpi);
				fgets(line, 200, fpi);
				//skip comments and blank lines
				while (line[0] == '#' || strlen(line) < 2)
					fgets(line, 200, fpi);
				sscanf(line, "%i %i", &width, &height);
				fgets(line, 200, fpi);
				//get pixel data
				int n = width * height * 3;
				data = new unsigned char[n];
				for (int i=0; i<n; i++)
					data[i] = fgetc(fpi);
				fclose(fpi);
			} else {
				printf("ERROR opening image: %s\n",ppmname);
				exit(0);
			}
			if (!ppmFlag)
				unlink(ppmname);
		}
};
Image img[14] = {"./seahorse.jpg", "./duck.jpeg", "./chowder.jpg", "./resize_dog.jpeg", "./grass.jpg", "./knight.png",
	"gameovertexture.jpg", "./menuscreen.jpg", "./zombie.png", "./orc.jpeg", "NKTitle.png","vampire.png", "./fence.png",
	"stone.png"};

unsigned char *buildAlphaData(Image *img)
{
	//add 4th component to RGB stream...
	int i;
	int a,b,c;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	for (i=0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		//-----------------------------------------------
		//get largest color component...
		//*(ptr+3) = (unsigned char)((
		//              (int)*(ptr+0) +
		//              (int)*(ptr+1) +
		//              (int)*(ptr+2)) / 3);
		//d = a;
		//if (b >= a && b >= c) d = b;
		//if (c >= a && c >= b) d = c;
		//*(ptr+3) = d;
		//-----------------------------------------------
		//this code optimizes the commented code above.
		*(ptr+3) = (a|b|c);
		//-----------------------------------------------
		ptr += 4;
		data += 3;
	}
	return newdata;
}

class Global {
	public:
		int xres, yres;
		char keys[65536];
		bool credits;
		bool pause;
		bool gameoverScreen = false;
		bool menuScreen = true;
		bool shipSpeedBoost = false;
		bool fireRateBoost = false;
		GLuint seahorseTexture;
		GLuint chowderTexture;
		GLuint duckTexture;
		GLuint menuTexture;
		GLuint jpcTexture;
		GLuint gameoverTexture;
		GLuint NKTitleTexture;
		GLuint backgroundTexture;
		GLuint playerTexture;
		GLuint zombieTexture;
		GLuint orcTexture;
		GLuint vampireTexture;
		GLuint woodTexture;
		GLuint stoneTexture;
		Global() {
			//Changed by Zakary Worman: Just made this resolution slightly larger
			xres = 1920;
			yres = 1080;
			//------------------------------------------------------------------
			memset(keys, 0, 65536);
			credits = false;
			pause = false;
		}
} gl;

class Ship {
	public:
		Vec dir;
		Vec pos;
		Vec vel;
		float angle;
		float color[3];
		int health = 3;
		int hit_recent = 0;
	public:
		Ship() {
			VecZero(dir);
			pos[0] = (Flt)(gl.xres/2);
			pos[1] = (Flt)(gl.yres/2);
			pos[2] = 0.0f;
			VecZero(vel);
			angle = 0.0;
			color[0] = color[1] = color[2] = 0.0f;
		}
};

class Bullet {
	public:
		Vec pos;
		Vec vel;
		float angle;
		float color[3];
		struct timespec time;
	public:
		Bullet() { }
};

class Game {
	public:
		Ship ship;
		//Asteroid *ahead;
		Bullet *barr;
		int round = 1;
		int enemyCount;
		int nasteroids;
		int nbullets;
		int killed = 0;;
		struct timespec bulletTimer;
		struct timespec mouseThrustTimer;
		bool roundEnd = true;
	public:
		Game() {
			//ahead = NULL;
			barr = new Bullet[MAX_BULLETS];
			nbullets = 0;
			clock_gettime(CLOCK_REALTIME, &bulletTimer);
		}
		~Game() {
			delete [] barr;
		}
} g;

//X Windows variables
class X11_wrapper {
	private:
		Display *dpy;
		Window win;
		GLXContext glc;
	public:
		X11_wrapper() {
			GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
			//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
			XSetWindowAttributes swa;
			setup_screen_res(gl.xres, gl.yres);
			dpy = XOpenDisplay(NULL);
			if (dpy == NULL) {
				std::cout << "\n\tcannot connect to X server" << std::endl;
				exit(EXIT_FAILURE);
			}
			Window root = DefaultRootWindow(dpy);
			XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
			if (vi == NULL) {
				std::cout << "\n\tno appropriate visual found\n" << std::endl;
				exit(EXIT_FAILURE);
			} 
			Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
			swa.colormap = cmap;
			swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
				PointerMotionMask | MotionNotify | ButtonPress | ButtonRelease |
				StructureNotifyMask | SubstructureNotifyMask;
			win = XCreateWindow(dpy, root, 0, 0, gl.xres, gl.yres, 0,
					vi->depth, InputOutput, vi->visual,
					CWColormap | CWEventMask, &swa);
			set_title();
			glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
			glXMakeCurrent(dpy, win, glc);
			show_mouse_cursor(0);
		}
		~X11_wrapper() {
			XDestroyWindow(dpy, win);
			XCloseDisplay(dpy);
		}
		void set_title() {
			//Set the window title bar.
			XMapWindow(dpy, win);
			XStoreName(dpy, win, "Asteroids template");
		}
		void check_resize(XEvent *e) {
			//The ConfigureNotify is sent by the
			//server if the window is resized.
			if (e->type != ConfigureNotify)
				return;
			XConfigureEvent xce = e->xconfigure;
			if (xce.width != gl.xres || xce.height != gl.yres) {
				//Window size did change.
				reshape_window(gl.xres, gl.yres);
			}
		}
		void reshape_window(int width, int height) {
			//window has been resized.
			setup_screen_res(width, height);
			glViewport(0, 0, (GLint)width, (GLint)height);
			glMatrixMode(GL_PROJECTION); glLoadIdentity();
			glMatrixMode(GL_MODELVIEW); glLoadIdentity();
			glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
			set_title();
		}
		void setup_screen_res(const int w, const int h) {
			gl.xres = w;
			gl.yres = h;
		}
		void swapBuffers() {
			glXSwapBuffers(dpy, win);
		}
		bool getXPending() {
			return XPending(dpy);
		}
		XEvent getXNextEvent() {
			XEvent e;
			XNextEvent(dpy, &e);
			return e;
		}
		void set_mouse_position(int x, int y) {
			XWarpPointer(dpy, None, win, 0, 0, 0, 0, x, y);
		}
		void show_mouse_cursor(const int onoff) {
			if (onoff) {
				//this removes our own blank cursor.
				XUndefineCursor(dpy, win);
				return;
			}
			//vars to make blank cursor
			//Zakary Worman: Changed to see mouse, will need to change it to a crosshair later
			Pixmap blank;
			XColor dummy;
			char data[1] = {0};
			Cursor cursor;
			//make a blank cursor
			blank = XCreateBitmapFromData (dpy, win, data, 1, 1);
			if (blank == None)
				std::cout << "error: out of memory." << std::endl;
			cursor = XCreatePixmapCursor(dpy, blank, blank, &dummy, &dummy, 0, 0);
			XFreePixmap(dpy, blank);
			//this makes you the cursor. then set it using this function
			XDefineCursor(dpy, win, cursor);
			//after you do not need the cursor anymore use this function.
			//it will undo the last change done by XDefineCursor
			//(thus do only use ONCE XDefineCursor and then XUndefineCursor):
		}
} x11;

//function prototypes
void init_opengl();
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics();
void render();

//==========================================================================
// M A I N
//==========================================================================
int main()
{
	logOpen();
	initSound();
	init_opengl();
	//initButtons();
	srand(time(NULL));
	//x11.set_mouse_position(100, 100);
	int done=0;
	while (!done) {
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			x11.check_resize(&e);
			check_mouse(&e);
			if(menuScreen()) {
				checkButtonClick(&e);
			}
			done = check_keys(&e);
			if (g.round >= 1)
				checkMouseEvent(&e, g.roundEnd);
		}
		physics();
		render();
		x11.swapBuffers();
	}
	cleanup_fonts();
	cleanupSound();
	logClose();
	return 0;
}

void init_opengl()
{
	//OpenGL initialization
	//-------------------------------------------------------------------------
	//seahorse
	//
	glGenTextures(1, &gl.seahorseTexture);
	int w = img[0].width;
	int h = img[0].height;
	//
	glBindTexture(GL_TEXTURE_2D, gl.seahorseTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[0].data);
	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------
	//duck
	//
	glGenTextures(1, &gl.duckTexture);
	w = img[1].width;
	h = img[1].height;
	//
	glBindTexture(GL_TEXTURE_2D, gl.duckTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[1].data);
	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------
	//chowder
	//
	glGenTextures(1, &gl.chowderTexture);
	w = img[2].width;
	h = img[2].height;
	//
	glBindTexture(GL_TEXTURE_2D, gl.chowderTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[2].data);
	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------
	//Menu Image
	glGenTextures(1, &gl.menuTexture);
	w = img[7].width;
	h = img[7].height;
	//
	glBindTexture(GL_TEXTURE_2D, gl.menuTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[7].data);
	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------
	//jpc
	//
	glGenTextures(1, &gl.jpcTexture);
	w = img[3].width;
	h = img[3].height;
	//
	glBindTexture(GL_TEXTURE_2D, gl.jpcTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[3].data);
	//-------------------------------------------------------------------------
	//gameoverimage
	//
	glGenTextures(1, &gl.gameoverTexture);
	w = img[6].width;
	h = img[6].height;
	//
	glBindTexture(GL_TEXTURE_2D, gl.gameoverTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[6].data);
	//-------------------------------------------------------------------------
	//Night Knight Title 
	//
	glGenTextures(1, &gl.NKTitleTexture);
	w = img[10].width;
	h = img[10].height;
	//
	glBindTexture(GL_TEXTURE_2D, gl.NKTitleTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[10].data);
	//-------------------------------------------------------------------------	


	glViewport(0, 0, gl.xres, gl.yres);



	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//This sets 2D mode (no perspective)
	glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
	//
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);
	//
	//Clear the screen to black
	glClearColor(0.0, 0.0, 0.0, 1.0);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------
	//User Sprite - JPC
	//
	glGenTextures(1, &gl.playerTexture);
	w = img[5].width;
	h = img[5].height;

	glBindTexture(GL_TEXTURE_2D, gl.playerTexture);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//must build a new set of data...*/
	unsigned char *silhouetteData = buildAlphaData(&img[5]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);
	//-------------------------------------------------------------------------



	//-------------------------------------------------------------------------

	glGenTextures(1, &gl.woodTexture);
	w = img[12].width;
	h = img[12].height;

	glBindTexture(GL_TEXTURE_2D, gl.woodTexture);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//must build a new set of data...*/
	silhouetteData = buildAlphaData(&img[12]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------

	glGenTextures(1, &gl.stoneTexture);
	w = img[13].width;
	h = img[13].height;

	glBindTexture(GL_TEXTURE_2D, gl.stoneTexture);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h,
	//                0, GL_RGB, GL_UNSIGNED_BYTE, img[5].data);

	//Trying to make sprite work - JPC

	//glGenTextures(1, &gl.playerSilhouette);

	//glBindTexture(GL_TEXTURE_2D, gl.playerSilhouette);
	//
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...*/
	silhouetteData = buildAlphaData(&img[13]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);
	//-------------------------------------------------------------------------
	//Zombie Sprite - Zakary
	//
	glGenTextures(1, &gl.zombieTexture);
	w = img[8].width;
	h = img[8].height;

	glBindTexture(GL_TEXTURE_2D, gl.zombieTexture);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	silhouetteData = buildAlphaData(&img[8]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	//Orc Sprite - Zakary
	//
	glGenTextures(1, &gl.orcTexture);
	w = img[9].width;
	h = img[9].height;

	glBindTexture(GL_TEXTURE_2D, gl.orcTexture);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	silhouetteData = buildAlphaData(&img[9]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);
	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------
	//Vampire Sprite - Zakary
	glGenTextures(1, &gl.vampireTexture);
	w = img[11].width;
	h = img[11].height;

	glBindTexture(GL_TEXTURE_2D, gl.vampireTexture);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	silhouetteData = buildAlphaData(&img[11]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);
	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------
	//Map Layout - JPC
	//

	glGenTextures(1, &gl.backgroundTexture);
	w = img[4].width;
	h = img[4].height;

	glBindTexture(GL_TEXTURE_2D, gl.backgroundTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h,
			0, GL_RGB, GL_UNSIGNED_BYTE, img[4].data);	
	//-------------------------------------------------------------------------

	initButtons();
}

void normalize2d(Vec v)
{
	Flt len = v[0]*v[0] + v[1]*v[1];
	if (len == 0.0f) {
		v[0] = 1.0;
		v[1] = 0.0;
		return;
	}
	len = 1.0f / sqrt(len);
	v[0] *= len;
	v[1] *= len;
}

void check_mouse(XEvent *e)
{
	//Was a mouse button clicked?
	//static int savex = 0;
	//static int savey = 0;
	//static int ct=0;      //changed by Zakary Worman:
	//these 3 variables not used because of my change of mouse aiming
	if (e->type != ButtonPress &&
			e->type != ButtonRelease &&
			e->type != MotionNotify)
		return;
	if (e->type == ButtonRelease)
		return;
	if (e->type == ButtonPress) {
		//edited by Zachary Kaiser: Messing around with bullet physics
		if (e->xbutton.button==1) {
			//Left button is down
			//a little time between each bullet
			struct timespec bt;
			clock_gettime(CLOCK_REALTIME, &bt);
			double ts = timeDiff(&g.bulletTimer, &bt);
			double fireRate = 0.7;
			if (!gl.fireRateBoost) {
				fireRate = 0.7;
			} else {
				fireRate = 0.3;
			}
			if (ts > fireRate) {
				timeCopy(&g.bulletTimer, &bt);
				//shoot a bullet...
				if (g.nbullets < MAX_BULLETS) {

					playSound();
					//playGameSound();
					Bullet *b = &g.barr[g.nbullets];
					timeCopy(&b->time, &bt);
					b->pos[0] = g.ship.pos[0];
					b->pos[1] = g.ship.pos[1];
					b->vel[0] = 0.5*g.ship.vel[0];
					b->vel[1] = 0.5*g.ship.vel[1];
					b->angle = g.ship.angle;
					//convert ship angle to radians
					Flt rad = ((g.ship.angle+90.0) / 360.0f) * PI * 2.0;
					//convert angle to a vector
					Flt xdir = cos(rad);
					Flt ydir = sin(rad);
					b->pos[0] += xdir*20.0f;
					b->pos[1] += ydir*20.0f;
					b->vel[0] += xdir*2 + rnd()*0.5;
					b->vel[1] += ydir*2 + rnd()*0.5;
					b->color[0] = 0.0f;
					b->color[1] = 0.0f;
					b->color[2] = 1.0f;
					++g.nbullets;
				}
			}
		}
	}
	if (e->type == MotionNotify) {
		//Mouse moved
		//Changed by Zakary Worman: Changed to remove movement from mouse
		//and allow for aiming with mouse. The rest of this usage is found
		//in physics
		int x = e->xbutton.x;           //just to save the x position of mouse
		int y = gl.yres - e->xbutton.y; //used to save the mouse y postion because
		//X11 and OpenGL start (0,0) in opposite
		//y positions 
		zk_savemouse(x, y);
		zw_save_mouse_pos(x, y);        //save this position to be used
	}
}

int check_keys(XEvent *e)
{
	//keyboard input?
	//static int shift=0;
	if (e->type != KeyPress && e->type != KeyRelease)
		return 0;
	int key = (XLookupKeysym(&e->xkey, 0) & 0x0000ffff);
	//Log("key: %i\n", key);
	if (e->type == KeyRelease) {
		gl.keys[key]=0;
		return 0;
	}
	gl.keys[key]=1;
	//Changed by Zakary Worman: Not used for anything
	switch (key) {
		case XK_Escape:
			return 1;
		case XK_Shift_L:
			g.ship.vel[0] *= 1.5;
			g.ship.vel[1] *= 1.5;
			break;
			//Added by Zakary Worman:
			//accelerates the unit as if a sprint
			//------------------------------------
		case XK_c:
			if (creditsScreen()) {
				closeCredits();
			} else {
				openCredits();
			}
			break;
		case XK_p:
			gl.pause = !gl.pause;
			break;
		case XK_d:
			break;
		case XK_equal:
			break;
		case XK_minus:
			break;
	}
	return 0;
}

void physics()
{
	//playGameSound();
	//Flt d0,d1,dist;
	//Update ship position
	if(!gl.shipSpeedBoost) {
		g.ship.pos[0] += g.ship.vel[0];
		g.ship.pos[1] += g.ship.vel[1];
	} else {
		g.ship.pos[0] += 2*g.ship.vel[0];
		g.ship.pos[1] += 2*g.ship.vel[1];
	}
	//Check for collision with window edges
	//Edited by Zachary Kaiser: Forced ship to stay within screen
	//boundaries
	if (g.ship.pos[0] <= 0.0) {
		g.ship.pos[0] = (float)gl.xres;
	}
	else if (g.ship.pos[0] >= (float)gl.xres) {
		g.ship.pos[0] = (float)gl.xres;
	}
	else if (g.ship.pos[1] <= 0.0) {
		g.ship.pos[1] = (float)gl.yres;
	}
	else if (g.ship.pos[1] >= (float)gl.yres) {
		g.ship.pos[1] = (float)gl.yres;
	}
	//
	//Update bullet positions
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	int i=0;
	while (i < g.nbullets) {
		Bullet *b = &g.barr[i];
		//How long has bullet been alive?
		//Edited by Zachary Kaiser: decreased amount of time to delete bullet
		double ts = timeDiff(&b->time, &bt);
		if (ts > 3.5) {
			//time to delete the bullet.
			memcpy(&g.barr[i], &g.barr[g.nbullets-1],
					sizeof(Bullet));
			g.nbullets--;
			//do not increment i.
			continue;
		}
		if(g.roundEnd) {
			g.nbullets--;
		}
		if(zw_check_enemy_hit(g.round, b->pos[0], b->pos[1])) {
			storeDeathPosition(b->pos[0],b->pos[1]);
			g.killed++;
			b->vel[0] *= -1;
			b->vel[1] *= -1;
			memcpy(&g.barr[i], &g.barr[g.nbullets-1], sizeof(Bullet));
			//powerupChance(powerups);
			g.nbullets--;
			g.enemyCount--;
			if(g.enemyCount == 0){
				gl.shipSpeedBoost = false;
				gl.fireRateBoost = false;
				resetPowerups();
				g.roundEnd = true;
			}
		} 
		//move the bullet
		b->pos[0] += b->vel[0];
		b->pos[1] += b->vel[1];
		//Check for collision with window edges
		//Edited by Zachary Kaiser: Deleted Bullet when it reaches screen edge
		if (b->pos[0] < 0.0) {
			memcpy(&g.barr[i], &g.barr[g.nbullets-1],
					sizeof(Bullet));
		}
		else if (b->pos[0] > (float)gl.xres) {
			memcpy(&g.barr[i], &g.barr[g.nbullets-1],
					sizeof(Bullet));
		}
		else if (b->pos[1] < 0.0) {
			memcpy(&g.barr[i], &g.barr[g.nbullets-1],
					sizeof(Bullet));
		}
		else if (b->pos[1] > (float)gl.yres) {
			memcpy(&g.barr[i], &g.barr[g.nbullets-1],
					sizeof(Bullet));
		}
		i++;
	}
	if(heartCollision(g.ship.pos[0], g.ship.pos[1])) {
		if(g.ship.health<3) {
			g.ship.health++;
		}
	}
	if(powerupCollision(g.ship.pos[0], g.ship.pos[1])) {
		if(checkSpeed()) {
			gl.shipSpeedBoost = true;
		}
		if(checkFireRate()) {
			gl.fireRateBoost = true;
		}
	}
	if(zw_player_hit(g.round, g.ship.pos[0], g.ship.pos[1])) {
		g.ship.vel[0] -= 5;
		g.ship.vel[1] -= 5;
		if(g.ship.hit_recent == 0) {
			g.ship.hit_recent = 10;
			g.ship.health--;
		}
		if(g.ship.hit_recent > 0)
			g.ship.hit_recent--;
	}
	//convert ship angle to radians
	Flt rad = ((g.ship.angle+90.0) / 360.0f) * PI * 2.0;
	//convert angle to a vector
	Flt xdir = cos(rad);
	Flt ydir = sin(rad);
	if (gl.keys[XK_a]) {
		g.ship.vel[0] -= sin(rad)*2;
		g.ship.vel[1] += cos(rad)*2;
	}
	else if (gl.keys[XK_d]) {
		g.ship.vel[0] += sin(rad)*2;
		g.ship.vel[1] -= cos(rad)*2;
	}
	if (gl.keys[XK_w]) {
		g.ship.vel[0] += xdir;
		g.ship.vel[1] += ydir;
		//Changed by Zakary Worman: changed to simply reduce the speed
		//to be more characteristic of a human rather than ship
	}
	else if (gl.keys[XK_s]) {
		//Added by Zakary Worman: this allows for backward movement with s
		g.ship.vel[0] -= xdir;
		g.ship.vel[1] -= ydir;
	}
	else {
		g.ship.vel[0] *= 0.8;
		g.ship.vel[1] *= 0.8;
	}
	//Changed by Zakary Worman: changed to simply reduce the speed
	//to be more characteristic of a human rather than ship
	Flt speed = sqrt(g.ship.vel[0]*g.ship.vel[0]+
			g.ship.vel[1]*g.ship.vel[1]);
	if (speed > 2.0f) {
		speed = 2.0f;
		normalize2d(g.ship.vel);
		g.ship.vel[0] *= speed;
		g.ship.vel[1] *= speed;
	}
	if (zw_player_structure_collision(g.ship.pos[0], g.ship.pos[1])) {
		g.ship.vel[0] *= -1;
		g.ship.vel[1] *= -1;
	}
	//Added by Zakary Worman: this makes the person slow down as you stop moving
	g.ship.angle = zw_change_angle(g.ship.pos[0], g.ship.pos[1]);
	if (!g.roundEnd)
		structureDamage();
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	if(creditsScreen()) {
		Rect n;
		n.bot = gl.yres - gl.yres/5;
		n.left = gl.xres/2;
		n.center = gl.xres/3;
		zw_show_credits(n);
		zk_show_credits(n);
		bb_show_credits(n);
		jc_show_credits(n);
		zwShowPicture(gl.xres - n.left/1.5, gl.yres - n.center/2.5, gl.seahorseTexture);
		zkShowPicture(gl.xres - n.left/1.5, gl.yres - n.center/1.3, gl.duckTexture);
		bbShowPicture(gl.xres - n.left/1.5, gl.yres - n.center/0.9, gl.chowderTexture);
		jpcShowPicture(gl.xres - n.left/1.5, gl.yres - n.center/0.675, gl.jpcTexture);
		ggprint16(&n, 16, 0x00ff0000, "Close this screen by pressing C");
		return;
	}
	if(menuScreen()) {
		menuScreenImage(gl.xres, gl.yres, gl.menuTexture, gl.NKTitleTexture);
		printMenuScreen(gl.xres, gl.yres);
		initButtons();
		drawButtons();
		zk_drawCircle();
		playGameSound();
		return;
	}
	if(g.ship.health <= 0) {
		zk_gameoverimage(gl.xres, gl.yres, gl.gameoverTexture);
		zk_gameovertext(gl.xres, gl.yres);
		if(gl.keys[XK_f]) {
			gl.menuScreen = true;
			g.ship.health = 3;
			g.roundEnd = true;
			g.round = 0;
			zw_reset_round();
			g.ship.hit_recent = 0;
			zk_resetpups(powerups);
		}	
		zk_drawCircle();
		//zw_gameover(gl.yres, gl.xres);
		return;
	}
	glClear(GL_COLOR_BUFFER_BIT);
	gameBackground(gl.xres, gl.yres, gl.backgroundTexture, gl.woodTexture, gl.stoneTexture, g.roundEnd);
	//buildPlacement(gl.xres, gl.yres, gl.woodTexture);
	Rect r;
	playerModel(g.ship.color, 3, g.ship.pos, 3, g.ship.angle, gl.playerTexture);
	if(gl.pause) {
		zk_pausemenu(gl.xres, gl.yres);
		zk_pausetext(gl.xres, gl.yres);
		zk_drawCircle();
		return;
	}
	if (g.roundEnd) {
		zk_drawCircle();
		zw_reset_round();
		Rect s;
		s.bot = gl.yres - 28;
		s.left = gl.xres/2 - 10;
		s.center = gl.xres/2;
		ggprint16(&s, 15, 0xcfcfcfcf, "Press r to start next round");
		if (g.round > 0)
			renderBoard(gl.xres, gl.yres, gl.woodTexture, gl.stoneTexture);
		//buildPlacement(gl.xres, gl.yres, gl.woodTexture);
		if (gl.keys[XK_r]) {
			g.round++;
			g.roundEnd = false;
			g.enemyCount = g.round*2;
			if (g.round > 4)
				g.enemyCount += (g.round-4)*2;
			if (g.round > 9)
				g.enemyCount += (g.round-9);
		}
		return;
	}
	//Drop Shadow
	Rect s;
	s.bot = gl.yres - 28;
	s.left = 10;
	s.center = 0;
	ggprint8b(&s, 16, 0x00000000, "3350 - Night Knight");
	ggprint8b(&s, 16, 0x00000000, "Arrows: %i", g.nbullets);
	ggprint8b(&s, 16, 0x00000000, "Enemies: %i", g.enemyCount);
	ggprint8b(&s, 16, 0x00000000, "Enemies Killed: %i", g.killed);
	s.bot = gl.yres - 28;
	s.left = gl.xres/2 - 10;
	s.center = gl.xres/2;
	ggprint16(&s, 16, 0xcfcfcfcf, "Round: %i", g.round);
	//
	r.bot = gl.yres - 20;
	r.left = 10;
	r.center = 0;
	ggprint8b(&r, 16, 0xaaffaaaa, "3350 - Night Knight");
	ggprint8b(&r, 16, 0x00ffff00, "Arrows: %i", g.nbullets);
	ggprint8b(&r, 16, 0xabababab, "Enemies: %i", g.enemyCount);
	ggprint8b(&r, 16, 0xaaaaaa22, "Enemies Killed: %i", g.killed);
	glBegin(GL_POINTS);
	glVertex2f(0.0f, 0.0f);
	glEnd();
	glPopMatrix();
	//draw shield
	if (g.ship.hit_recent > 0) {
		glColor3f(1.0f,1.0f,0.0f);
		glPushMatrix();
		glTranslatef(g.ship.pos[0], g.ship.pos[1], g.ship.pos[2]);
		glRotatef(g.ship.angle, 0.0f, 0.0f, 1.0f);
		glBegin(GL_LINE_LOOP);
		glVertex2f(-30, -40);           
		glVertex2f(-30, 40);           
		glVertex2f(30, 40);           
		glVertex2f(30, -40);           
		glEnd();
		glPopMatrix();
		Rect k;
		k.bot = gl.yres/1.2;
		k.left = gl.xres/2 - 10;
		k.center = gl.xres/2;
		ggprint16(&k, 16, 0xffffff00, "SHIELDED %i", g.ship.hit_recent);
	}
	//Draw the enemies
	zw_spawn_enemies(g.round, g.ship.pos[0], g.ship.pos[1], gl.zombieTexture, gl.orcTexture, gl.vampireTexture);
	//Draw the bullets
	Bullet *b = &g.barr[0];
	for (int i=0; i<g.nbullets; i++) {
		zw_drawSword(b->pos[0], b->pos[1], b->angle);
		++b;
	}
	//gameBackground(gl.xres, gl.yres, gl.backgroundTexture);
	//Draw Circle over Crosshair, Zachary Kaiser
	zk_drawCircle();
	zk_blackbar();
	renderHealth(g.ship.health);
	zk_showhealthtext(gl.xres, gl.yres);
	drawHeart();
	drawPowerups();
}
