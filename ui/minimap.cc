#include "ui/minimap.h"

#include "./globals.h"
#include "engine/world.h"
#include "ui/ui.h"

Minimap::Minimap(int w, int h)
	: W(w), H(h)
{
}


Minimap::~Minimap()
{
}


void
Minimap::Initialize()
{
	int limit_x1, limit_y1, limit_x2, limit_y2;
	world->Limits(limit_x1, limit_y1, limit_x2, limit_y2);

	CreateImage(W, H);

	// draw tiles
	/*
	for(int x=0; x<W; x++) for(int y=0; y<H; y++) {
		Block const* b[10];
		world->Tiles(b, (-limit_x1+limit_x2) / W * (x-W/2),
				(-limit_y1+limit_y2) / H * (y-H/2));
		DrawPoint(x, y, b[0]->R, b[0]->G, b[0]->B);
	}*/

	// TODO - draw rivers
	
	// TODO - draw cities
	FinishImage();
}


void 
Minimap::Draw(int x, int y) const
{
	PresentImage(x, y);
}
