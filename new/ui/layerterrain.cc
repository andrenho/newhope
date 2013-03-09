#include "defines.h"

#include <GL/gl.h>

void
LayerTerrain::Render() const
{
	Reference r = ui->Imageset()["grass_c"];

	glLoadIdentity();
	glTranslatef(5.0f, 5.0f, 0.0f);

	glBindTexture(GL_TEXTURE_2D, ui->Imageset().Texture()[r.idx]);

	glBegin(GL_QUADS);
	  glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
	  glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f, 0.0f);
	  glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f, 0.0f);
	  glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f, 0.0f);
	glEnd();
}
