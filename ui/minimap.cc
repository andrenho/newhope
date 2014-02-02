#include "ui/minimap.h"

#include "./globals.h"
#include "engine/world.h"
#include "ui/ui.h"

Minimap::Minimap(int w, int h)
	: W(w), H(h)
{
	int limit_x1, limit_y1, limit_x2, limit_y2;
	world->Limits(limit_x1, limit_y1, limit_x2, limit_y2);

	CreateImage(w, h);

	for(int x=0; x<w; x++) for(int y=0; y<h; y++) {
		Block const* b[10];
		world->Tiles(b, (-limit_x1+limit_x2) / w * (x-w/2),
				(-limit_y1+limit_y2) / h * (y-h/2));

	}
}


Minimap::~Minimap()
{
}


void 
Minimap::Draw(int x, int y)
{
}
