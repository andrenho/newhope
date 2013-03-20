#include "defines.h"

#include <GL/gl.h>
#include <GL/glfw.h>

void 
Scene::Initialize() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void 
Scene::Render() const
{
	glfwSwapBuffers();
}


void 
Scene::setProportion(ProportionScene ps) const
{
	if(ps == PROP_TILES) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0.0f, ui->WindowW()/(ui->Zoom()*16.0), 
			   0.0f, ui->WindowH()/(ui->Zoom()*16.0));
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(ui->RelX, 
			float(ui->WindowH())/(ui->Zoom()*16.0f) - 1.0f + ui->RelY, 
			0.0f);
	} else {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0.0f, ui->WindowW()/ui->Zoom(), 
			   0.0f, ui->WindowH()/ui->Zoom());
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0.0f, 0.0f, 0.0f);
	}
	ps_ = ps;
}


void 
Scene::DrawImage(string index, float x, float y) const
{
	float px = float(x);
	float py = -float(y);
	Reference rc = ui->Imageset()[index];

	float w = 1.0f, h = 1.0f;
	if(ps_ == PROP_WINDOW) {
		w = rc.real_w; 
		h = rc.real_h;
		py = ui->WindowH() / ui->Zoom() + py - 16;
	}

	glBindTexture(GL_TEXTURE_2D, ui->Imageset().Texture()[rc.idx]);
	glBegin(GL_QUADS);
	  glTexCoord2f(rc.x,      rc.y+rc.h); glVertex3f(px,   py,   0.0f);
	  glTexCoord2f(rc.x+rc.w, rc.y+rc.h); glVertex3f(px+w, py,   0.0f);
	  glTexCoord2f(rc.x+rc.w, rc.y);      glVertex3f(px+w, py+h, 0.0f);
	  glTexCoord2f(rc.x,      rc.y);      glVertex3f(px,   py+h, 0.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
}


void 
Scene::DrawRectangle(float x1, float y1, float x2, float y2, Color c) const
{
	if(ps_ == PROP_WINDOW) {
		y1 = ui->WindowH() / ui->Zoom() - y1;
		y2 = ui->WindowH() / ui->Zoom() - y2;
	}

	glColor3f(float(c.r) / 255.0f, float(c.g) / 255.0f, float(c.b) / 255.0f);
	glBegin(GL_QUADS);
	  glVertex3f(x1, y1, 0.0f);
	  glVertex3f(x2, y1, 0.0f);
	  glVertex3f(x2, y2, 0.0f);
	  glVertex3f(x1, y2, 0.0f);
	glEnd();
}


void 
Scene::WindowTileBorders(int& x1, int& y1, int& x2, int& y2) const
{
	x1 = (int)(-ui->RelX-1);
	y1 = (int)(ui->RelY-1);
	x2 = (int)(-ui->RelX-1)+(ui->WindowW()/ui->Zoom()/16.0f)+3;
	y2 = (int)(ui->RelY-1)+(ui->WindowH()/ui->Zoom()/16.0f)+3;
}
