#include "defines.h"

#include <GL/gl.h>

void
LayerTerrain::Render() const
{
	glLoadIdentity();
	glTranslatef(ui->RelX, 
		float(ui->WindowH())/(ui->Zoom()*16.0f) - 1.0f + ui->RelY, 
		0.0f);

	// TODO - only draw the part on the screen
	for(int x=0; x<40; x++) {
		for(int y=0; y<40; y++) {
			DrawTile(x, y);
		}
	}
}

void
LayerTerrain::DrawTile(int x, int y) const
{
	string suffix = TileSuffix(x, y);
	if(suffix == "") {
		return;
	}
	Reference rc = ui->Imageset()[TerrainStr() + "_" + suffix];

	glBindTexture(GL_TEXTURE_2D, ui->Imageset().Texture()[rc.idx]);

	float px = float(x);
	float py = -float(y);

	glBegin(GL_QUADS);
	  glTexCoord2f(rc.x,      rc.y+rc.h); glVertex3f(px,   py,   0.0f);
	  glTexCoord2f(rc.x+rc.w, rc.y+rc.h); glVertex3f(px+1, py,   0.0f);
	  glTexCoord2f(rc.x+rc.w, rc.y);      glVertex3f(px+1, py+1, 0.0f);
	  glTexCoord2f(rc.x,      rc.y);      glVertex3f(px,   py+1, 0.0f);
	glEnd();
}


string 
LayerTerrain::TileSuffix(int x, int y) const
{
	if(game->Map().Terrain(x, y) == terrain_) {
		return "c";
	}
	return "";
}


string 
LayerTerrain::TerrainStr() const
{
	switch(terrain_)
	{
	case t_GRASS: return "grass";
	case t_WATER: return "water";
	default: throw new ui_error("Invalid terrain " + terrain_);
	}
}
