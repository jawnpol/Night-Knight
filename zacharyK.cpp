//Zachary Kaiser


#include"fonts.h" 
#include<stdio.h>
#include<string.h>
#include<GL/glx.h>
#include<cstdlib>
#include<math.h>

#define MAXBUTTONS 2

typedef struct buttn {
        Rect btn;
        char text[32];
        int hover;
        int pressed;
        int click;
        float btnColor[3];
        float pColor[3];
        unsigned int text_color;
} Button;

struct Bottn {
        Button button[MAXBUTTONS];
        int nbuts;
        bool pause;
} glo;

struct Trackmouse {
	float x, y;
} k;

void zk_initializeButtons() {
        glo.nbuts = 0;
        glo.button[glo.nbuts].btn.width = 240;
        glo.button[glo.nbuts].btn.height = 120;
        glo.button[glo.nbuts].btn.left = 840;
        glo.button[glo.nbuts].btn.bot = 400;
        glo.button[glo.nbuts].btn.right =
                glo.button[glo.nbuts].btn.left + glo.button[glo.nbuts].btn.width;
        glo.button[glo.nbuts].btn.top =
                glo.button[glo.nbuts].btn.bot + glo.button[glo.nbuts].btn.height;
        glo.button[glo.nbuts].btn.centerx =
                (glo.button[glo.nbuts].btn.left + glo.button[glo.nbuts].btn.right) / 2;
        glo.button[glo.nbuts].btn.centery =
                (glo.button[glo.nbuts].btn.bot + glo.button[glo.nbuts].btn.top) / 2;
        strcpy(glo.button[glo.nbuts].text, "Continue");
        glo.button[glo.nbuts].pressed = 0;
        glo.button[glo.nbuts].click = 0;
        glo.button[glo.nbuts].btnColor[0] = 0.5f;
        glo.button[glo.nbuts].btnColor[1] = 0.5f;
        glo.button[glo.nbuts].btnColor[2] = 0.5f;
        /*glo.button[glo.nbuts].pColor[0] = 1.0f;
          glo.button[glo.nbuts].pColor[1] = 1.0f;
          glo.button[glo.nbuts].pColor[2] = 1.0f;*/
        glo.button[glo.nbuts].text_color = 0x000000;

        glo.nbuts++;

        glo.button[glo.nbuts].btn.width = 240;
        glo.button[glo.nbuts].btn.height = 120;
        glo.button[glo.nbuts].btn.left = 840;
        glo.button[glo.nbuts].btn.bot = 600;
        glo.button[glo.nbuts].btn.right =
                glo.button[glo.nbuts].btn.left + glo.button[glo.nbuts].btn.width;
        glo.button[glo.nbuts].btn.top =
                glo.button[glo.nbuts].btn.bot + glo.button[glo.nbuts].btn.height;
        glo.button[glo.nbuts].btn.centerx =
                (glo.button[glo.nbuts].btn.left + glo.button[glo.nbuts].btn.right) / 2;
        glo.button[glo.nbuts].btn.centery =
                (glo.button[glo.nbuts].btn.bot + glo.button[glo.nbuts].btn.top) / 2;
        strcpy(glo.button[glo.nbuts].text, "Controls");
        glo.button[glo.nbuts].pressed = 0;
        glo.button[glo.nbuts].click = 0;
        glo.button[glo.nbuts].btnColor[0] = 0.5f;
        glo.button[glo.nbuts].btnColor[1] = 0.5f;
        glo.button[glo.nbuts].btnColor[2] = 0.5f;
        /*glo.button[glo.nbuts].pColor[0] = 1.0f;
          glo.button[glo.nbuts].pColor[1] = 1.0f;
          glo.button[glo.nbuts].pColor[2] = 1.0f;*/
        glo.button[glo.nbuts].text_color = 0x000000;

        glo.nbuts++;
	
	/*glo.button[glo.nbuts].btn.width = 240;
        glo.button[glo.nbuts].btn.height = 120;
        glo.button[glo.nbuts].btn.left = 180;
        glo.button[glo.nbuts].btn.bot = 400;
        glo.button[glo.nbuts].btn.right =
                glo.button[glo.nbuts].btn.left + glo.button[glo.nbuts].btn.width;
        glo.button[glo.nbuts].btn.top =
                glo.button[glo.nbuts].btn.bot + glo.button[glo.nbuts].btn.height;
        glo.button[glo.nbuts].btn.centerx =
                (glo.button[glo.nbuts].btn.left + glo.button[glo.nbuts].btn.right) / 2;
        glo.button[glo.nbuts].btn.centery =
                (glo.button[glo.nbuts].btn.bot + glo.button[glo.nbuts].btn.top) / 2;
        strcpy(glo.button[glo.nbuts].text, "Return");
        glo.button[glo.nbuts].pressed = 0;
        glo.button[glo.nbuts].click = 0;
        glo.button[glo.nbuts].btnColor[0] = 0.5f;
        glo.button[glo.nbuts].btnColor[1] = 0.5f;
        glo.button[glo.nbuts].btnColor[2] = 0.5f;
        glo.button[glo.nbuts].pColor[0] = 1.0f;
          glo.button[glo.nbuts].pColor[1] = 1.0f;
          glo.button[glo.nbuts].pColor[2] = 1.0f;
        glo.button[glo.nbuts].text_color = 0x000000;

        glo.nbuts++;*/
}

void zk_createButtons() {
        Rect r;
        for(int i = 0; i < glo.nbuts; i++) {
                if(glo.button[i].hover==1) {
                        int w=2;
                        glColor3f(1.0f, 1.0f, 1.0f);
                        glLineWidth(3);
                        glBegin(GL_LINE_LOOP);
                        glVertex2i(glo.button[i].btn.left-w, glo.button[i].btn.bot-w);
                        glVertex2i(glo.button[i].btn.left-w, glo.button[i].btn.top+w);
                        glVertex2i(glo.button[i].btn.right+w, glo.button[i].btn.top+w);
                        glVertex2i(glo.button[i].btn.right+w, glo.button[i].btn.bot-w);
                        glVertex2i(glo.button[i].btn.left-w, glo.button[i].btn.bot-w);
                        glEnd();
                        glLineWidth(1);
                }
                if(glo.button[i].pressed) {
                        glColor3fv(glo.button[i].pColor);
                } else {
                        glColor3fv(glo.button[i].btnColor);
                }
                glBegin(GL_QUADS);
                glVertex2i(glo.button[i].btn.left, glo.button[i].btn.bot);
                glVertex2i(glo.button[i].btn.left, glo.button[i].btn.top);
                glVertex2i(glo.button[i].btn.right, glo.button[i].btn.top);
                glVertex2i(glo.button[i].btn.right, glo.button[i].btn.bot);
                glEnd();
                r.left = glo.button[i].btn.centerx;
                r.bot = glo.button[i].btn.centery-8;
                r.center = 1;
                ggprint16(&r, 0, glo.button[i].text_color, glo.button[i].text);
        }
}

void zk_checkClick(XEvent *e)   {
        int leftclick = 0;
        int x,y;
        if (e->type == ButtonPress) {
                if (e->xbutton.button == 1) {
                        leftclick = 1;
                }
        }
        x = e->xbutton.x;
        y = e->xbutton.y;
        y = 1080 - y;
        for (int i = 0; i < glo.nbuts; i++) {
                glo.button[i].hover = 0;
                if (x >= glo.button[i].btn.left &&
                                x <= glo.button[i].btn.right &&
                                y >= glo.button[i].btn.bot &&
                                y <= glo.button[i].btn.top) {
                        glo.button[i].hover = 1;
                        //printf("over button %i\n", i);fflush(stdout);
                        if (glo.button[i].hover == 1) {
                                if (leftclick == 1) {
                                        switch(i) {
                                                case 0:
                                                        //printf("button clicked!\n");fflush(stdout);
                                                        glo.pause = !glo.pause;
                                                        break;
                                        }
                                }
                        }
                }
        }

        if (e->type == MotionNotify) {
                if (x >= glo.button[0].btn.left &&
                                x <= glo.button[0].btn.right &&
                                y >= glo.button[0].btn.bot &&
                                y <= glo.button[0].btn.top) {
                        glo.button[0].hover = 1;
                        //printf("hovering\n");fflush(stdout);
                } else {
                        glo.button[0].hover = 0;
                }
        }
}



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

void zk_showhealthtext() {
	Rect r;
	r.bot = 165;
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

void zk_pausemenu(int x, int y) {
	glColor3f(0, 0, 0);

	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(0, y);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(x, y);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(x, 0);
	glEnd();
	glPopMatrix();
}
bool zk_continue() {
	if(k.x >= glo.button[0].btn.left &&
			k.x <= glo.button[0].btn.right &&
			k.y >= glo.button[0].btn.bot &&
			k.y <= glo.button[0].btn.top) {
		return true;
	}
	return false;	
}

bool zk_cState() {
	if(k.x >= glo.button[2].btn.left &&
			k.x <= glo.button[2].btn.right &&
			k.y >= glo.button[2].btn.bot &&
			k.y <= glo.button[2].btn.top) 
		return true;
	return false;

}

void zk_controls (int x, int y) {
        glColor3f(0, 0, 0);

        glPushMatrix();
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(0, y);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(x, y);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(x, 0);
        glEnd();
        glPopMatrix();
}

void zk_ctext(int x, int y) {
        Rect r;

        r.bot = y - y/3;
        r.left = x/2;
        r.center = x/3;
}
void zk_pausetext(int x, int y) {
        Rect r;

        r.bot = y - y/5;
        r.left = x/2;
        r.center = x/3;

        ggprint16(&r, 16, 0x00ff0000, "Press P to continue.");
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


void zk_savemouse(int x, int y) {
	k.x = x;
	k.y = y;
}
