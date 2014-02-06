#include "ui/minimap.h"

#include "./globals.h"
#include "engine/hero.h"
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

	int prop_w = limit_x1 / (limit_x2 - limit_x1),
	    prop_h = limit_y1 / (limit_y2 - limit_y1);

	CreateImage(W, H);

	// draw tiles
	for(int x=0; x<W; x++) for(int y=0; y<H; y++) {
		Block const* b[10];
		world->Tiles(b, (-limit_x1+limit_x2) / W * (x-W/2),
				(-limit_y1+limit_y2) / H * (y-H/2));
		DrawPoint(x, y, b[0]->R, b[0]->G, b[0]->B);
	}

	// draw rivers
	for(auto const& river : world->Rivers()) {
		std::vector<Point> pts;
		for(auto const& p : river) {
			pts.push_back(Point(
				(p.X() / (limit_x2 - limit_x1) - prop_w) * W + W/2,
				(p.Y() / (limit_y2 - limit_y1) - prop_h) * H + H/2));
		}
		DrawPoints(pts, Block::WATER->R, Block::WATER->G, Block::WATER->B);
	}
	
	// TODO - draw cities
	FinishImage();
}


void 
Minimap::Draw(int x, int y) const
{
	// present image
	PresentImage(x, y);

	int limit_x1, limit_y1, limit_x2, limit_y2;
	world->Limits(limit_x1, limit_y1, limit_x2, limit_y2);

	int prop_w = limit_x1 / (limit_x2 - limit_x1),
	    prop_h = limit_y1 / (limit_y2 - limit_y1);

	// draw player
	Point const& pos = world->Hero().Position();
	int px = static_cast<int>((pos.X() / (limit_x2 - limit_x1) - prop_w) * W + x + W/2);
	int py = static_cast<int>((pos.Y() / (limit_y2 - limit_y1) - prop_h) * H + y + H/2);
	DrawRectangle(px-2, py-2, 5, 5, true, 255, 255, 0);
	DrawRectangle(px-3, py-3, 7, 7, false, 0, 0, 0);

	UpdateScreen();
}
