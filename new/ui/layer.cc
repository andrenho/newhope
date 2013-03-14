#include "defines.h"

void 
Layer::Draw(Reference rc, int x, int y) const
{
	glBindTexture(GL_TEXTURE_2D, ui->Imageset().Texture()[rc.idx]);

	float px = float(x) / 16.0f;
	float py = -float(y) / 16.0f;
	glBegin(GL_QUADS);
	  glTexCoord2f(rc.x, rc.y+rc.h); glVertex3f(px, py, 0.0f);
	  glTexCoord2f(rc.x+rc.w, rc.y+rc.h); glVertex3f(px+1, py, 0.0f);
	  glTexCoord2f(rc.x+rc.w, rc.y); glVertex3f(px+1, py+1, 0.0f);
	  glTexCoord2f(rc.x, rc.y); glVertex3f(px, py+1, 0.0f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
}
