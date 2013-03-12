#include "defines.h"

#include <GL/gl.h>

void
LayerTerrain::Render() const
{
	glLoadIdentity();
	glTranslatef(ui->RelX, 
		float(ui->WindowH())/(ui->Zoom()*16.0f) - 1.0f + ui->RelY, 
		0.0f);

	int left = (int)(-ui->RelX-1)+(ui->WindowW()/ui->Zoom()/16.0f)+3;
	int bottom = (int)(ui->RelY-1)+(ui->WindowH()/ui->Zoom()/16.0f)+3;
	for(int x=(int)(-ui->RelX-1); x<left; x++) {
		for(int y=(int)(ui->RelY-1); y<bottom; y++) {
			DrawTile(x, y);
		}
	}
}

void
LayerTerrain::DrawTile(int x, int y) const
{
	vector<string> suffixes;
	TileSuffixes(x, y, suffixes);
	if(suffixes.empty()) {
		return;
	}

	for(auto const& suffix: suffixes) {
		Reference rc = ui->Imageset()[TerrainStr() + "_" + suffix];

		glBindTexture(GL_TEXTURE_2D, ui->Imageset().Texture()[rc.idx]);

		float px = float(x);
		float py = -float(y);
		glBegin(GL_QUADS);
		  glTexCoord2f(rc.x, rc.y+rc.h); glVertex3f(px, py, 0.0f);
		  glTexCoord2f(rc.x+rc.w, rc.y+rc.h); glVertex3f(px+1, py, 0.0f);
		  glTexCoord2f(rc.x+rc.w, rc.y); glVertex3f(px+1, py+1, 0.0f);
		  glTexCoord2f(rc.x, rc.y); glVertex3f(px, py+1, 0.0f);
		glEnd();
	}
}


void
LayerTerrain::TileSuffixes(int x, int y, vector<string>& s) const
{
	Map const& m = game->Map();

	// central tile
	if(m.Terrain(x, y) == terrain_) {
		s.push_back("c");
		return;
	}

	// inner corners
	if(m.Terrain(x-1, y) == terrain_ && m.Terrain(x, y-1) == terrain_) {
		s.push_back("corner_nw");
	}
	if(m.Terrain(x+1, y) == terrain_ && m.Terrain(x, y-1) == terrain_) {
		s.push_back("corner_ne");
	}
	if(m.Terrain(x-1, y) == terrain_ && m.Terrain(x, y+1) == terrain_) {
		s.push_back("corner_sw");
	}
	if(m.Terrain(x+1, y) == terrain_ && m.Terrain(x, y+1) == terrain_) {
		s.push_back("corner_se");
	}

	// outer corners
	if(m.Terrain(x-1, y-1) == terrain_ 
	&& m.Terrain(x, y-1) != terrain_ && m.Terrain(x-1, y) != terrain_) {
		s.push_back("se");
	}
	if(m.Terrain(x+1, y-1) == terrain_ 
	&& m.Terrain(x, y-1) != terrain_ && m.Terrain(x+1, y) != terrain_) {
		s.push_back("sw");
	}
	if(m.Terrain(x-1, y+1) == terrain_ 
	&& m.Terrain(x, y+1) != terrain_ && m.Terrain(x-1, y) != terrain_) {
		s.push_back("ne");
	}
	if(m.Terrain(x+1, y+1) == terrain_ 
	&& m.Terrain(x, y+1) != terrain_ && m.Terrain(x+1, y) != terrain_) {
		s.push_back("nw");
	}

	// sides
	if(m.Terrain(x-1, y) == terrain_
	&& m.Terrain(x, y-1) != terrain_ && m.Terrain(x, y+1) != terrain_) {
		s.push_back("e");
	}
	if(m.Terrain(x+1, y) == terrain_
	&& m.Terrain(x, y-1) != terrain_ && m.Terrain(x, y+1) != terrain_) {
		s.push_back("w");
	}
	if(m.Terrain(x, y-1) == terrain_
	&& m.Terrain(x-1, y) != terrain_ && m.Terrain(x+1, y) != terrain_) {
		s.push_back("s");
	}
	if(m.Terrain(x, y+1) == terrain_
	&& m.Terrain(x-1, y) != terrain_ && m.Terrain(x+1, y) != terrain_) {
		s.push_back("n");
	}
}


string 
LayerTerrain::TerrainStr() const
{
	switch(terrain_)
	{
	case t_GRASS: return "grass";
	case t_WATER: return "water";
	default: throw new ui_error("Invalid terrain");
	}
}
