//Zachary Kaiser


#include"fonts.h" 
#include<stdio.h>
#include<GL/glx.h>
#include<cstdlib>
#include<math.h>

struct Trackmouse {
	float x, y;
} k;

void zk_show_credits(Rect &r) {
   ggprint16(&r, 150, 0x00fff000, "Zachary Kaiser");  
}

void zkShowPicture(int x, int y, GLuint texid) {
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

void zk_drawCircle() {
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef(k.x, k.y, 0.0f);
	glBegin(GL_LINE_LOOP);
	for(int i = 0; i < 360; i++) {
		glVertex2f(10*sinf(i*3.14/180), 10*cosf(i*3.14/180));
	}
	glEnd();
	glColor3f(1.0f,0.0f,0.0f);
	glBegin(GL_POINTS);
	glVertex2f(0.0f, 0.0f);
	glEnd();
	glPopMatrix();
}

void zk_gameovertext(int x, int y) {
	Rect r;
	r.bot = y - y/6;
	r.left = x/2;
	r.center = x/3;

	ggprint16(&r, 16, 0x00ff0000, "Night Night Knight.");
	ggprint16(&r, 16, 0x00ff0000, "Press F to pay respects, and also play again.");
}

void zk_showhealthtext(int x, int y) {
	Rect r;
	r.bot = 175;
	r.left = 25;

	ggprint16(&r, 16, 0x00ffffff, "HP: ");
}

void zk_blackbar() {
	glBegin(GL_QUADS);
	glColor3f(0, 0, 0);
	glVertex2f(0, 0);
	glVertex2f(55, 0);
	glVertex2f(55, 155);
	glVertex2f(0, 155);
	glEnd();
}

void zk_gameoverimage(int x, int y, GLuint texid) {
    	glColor3ub(255, 255, 255);

    	//glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texid);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(0, y);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(x, y);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(x, 0);
	glEnd();
	glPopMatrix();
}

void zk_resetpups(int arr[]) {
	for(int i=0; i<5; i++) {
		arr[i] = 0;
	}
}
void zk_savemouse(int x, int y) {
	k.x = x;
	k.y = y;
}
