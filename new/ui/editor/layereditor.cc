#include "defines.h"

#include <cstdlib>

struct LayerType {
	int layer;
	vector<string> tiles;
};
static const map<char, LayerType> layers = {
	{ 'T', { 1, { "grass_c", "water_c" } } },
};

void 
LayerEditor::Render() const
{
	// setup
	glLoadIdentity();
	glTranslatef(0.0f, 
		float(ui->WindowH())/(ui->Zoom()*16.0f) - 1.0f, 
		0.0f);

	// draw black rectangle
	float px = float(((ui->WindowW() / ui->Zoom()) - 80) / 16.0f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	  glVertex3f(px, -ui->WindowH(), 0.0f);
	  glVertex3f(px+80, -ui->WindowH(), 0.0f);
	  glVertex3f(px+80, 16, 0.0f);
	  glVertex3f(px, 16, 0.0f);
	glEnd();

	// draw tiles
	float x = 0, y = 0;
	try {
		for(auto const& tile: layers.at(option_).tiles) {
			Reference rc = ui->Imageset()[tile];
			Draw(rc, (x + px) * 16 + 8, 8 - (y*16));
			x += 1.5f;
		}
	} catch(out_of_range& e) { 
		abort();
	}
}
