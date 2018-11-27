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
	r.bot = y - y/5;
	r.left = x/2;
	r.center = x/3;

	ggprint16(&r, 16, 0x00ff0000, "Night Night Knight.");
	ggprint16(&r, 16, 0x00ff0000, "Press F to pay respects, and also play again.");
}


void zk_gameoverimage(int x, int y, GLuint texid) {
    glColor3ub(255,255,255);

	glPushMatrix();
	glTranslatef((float)x, (float)y, 0);
	glBindTexture(GL_TEXTURE_2D, texid);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(-x/2, -y/2);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(-x/2, y/2);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(x/2, y/2);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(x/2, -y/2);
	glEnd();
	glPopMatrix();
}

void zk_savemouse(int x, int y) {
	k.x = x;
	k.y = y;
}
