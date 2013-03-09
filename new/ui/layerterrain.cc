#include "defines.h"

#include <GL/gl.h>

void
LayerTerrain::Render() const
{
	Reference r = ui->Imageset()["grass_c"];

	glLoadIdentity();
	glTranslatef(1.0f, 1.0f, 0.0f);

	glBindTexture(GL_TEXTURE_2D, ui->Imageset().Texture()[r.idx]);

	glBegin(GL_QUADS);
	  glTexCoord2f(r.x,     r.y+r.h); glVertex3f(-1.0f, -1.0f, 0.0f);
	  glTexCoord2f(r.x+r.w, r.y+r.h); glVertex3f( 1.0f, -1.0f, 0.0f);
	  glTexCoord2f(r.x+r.w, r.y);     glVertex3f( 1.0f,  1.0f, 0.0f);
	  glTexCoord2f(r.x    , r.y);     glVertex3f(-1.0f,  1.0f, 0.0f);
	glEnd();
}
