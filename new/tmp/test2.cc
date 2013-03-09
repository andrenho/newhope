#include <iostream>
#include <cstdlib>

using namespace std;

#include <GL/gl.h>
#include <GL/glu.h>
#include "SDL.h"
#include "SDL_image.h"

SDL_Surface* scr;
GLuint texture[1];


static void loadTextures()
{
	SDL_Surface* text_sf[1];
	if((text_sf[0] = SDL_LoadBMP("test.bmp"))) {
	//if((text_sf[0] = IMG_Load("test.png"))) {
		glGenTextures(1, &texture[0]);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, text_sf[0]->w,
				text_sf[0]->h, 0, GL_BGR,
				GL_UNSIGNED_BYTE, text_sf[0]->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	} else {
		cerr << "Cannot load image:" << SDL_GetError() << endl;
		exit(-1);
	}
	SDL_FreeSurface(text_sf[0]);
}


static void initGL()
{
	loadTextures();

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	//glShadeModel(GL_SMOOTH);
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
	//glClearDepth(1.0f);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//gluOrtho2D(0.0f, 300.0f, 0.0f, 300.0f);
}


static void drawGLScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glTranslatef(5.0f, 5.0f, -0.0f);

	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glBegin(GL_QUADS);
	  glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
	  glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f, 0.0f);
	  glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f, 0.0f);
	  glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f, 0.0f);
	glEnd();

	glTranslatef(2.0f, 0.0f, 0.5f);
	glBegin(GL_QUADS);
	  glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
	  glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f, 0.0f);
	  glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f, 0.0f);
	  glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f, 0.0f);
	glEnd();

	SDL_GL_SwapBuffers();
}


static void resizeWindow(int w, int h)
{
	GLfloat ratio = (GLfloat)w / (GLfloat)h;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0f, w/16.0, 0.0f, h/16.0);
	//gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	scr = SDL_SetVideoMode(300, 300, 0,
			SDL_OPENGL|SDL_GL_DOUBLEBUFFER|SDL_HWPALETTE|
			SDL_RESIZABLE|SDL_HWSURFACE|SDL_HWACCEL);
	if(!scr) {
		cerr << "Could not open SDL window: " << SDL_GetError() << endl;
		return -1;
	}

	initGL();
	resizeWindow(300, 300);
	
	SDL_Event e;
	bool run = true;
	while(run) {
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) {
				run = false;
			}
		}
		drawGLScene();
	}
}
