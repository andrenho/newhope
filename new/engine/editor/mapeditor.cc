#include "defines.h"

#include <algorithm>

MapEditor::MapEditor()
	: Map(), tiles_(nullptr)
{
	Resize(50, 25);
}


MapEditor::~MapEditor()
{
	if(tiles_) {
		delete[] tiles_;
		tiles_ = nullptr;
	}
}


enum Terrain 
MapEditor::Terrain(int x, int y) const 
{ 
	if(x < 0 || y < 0 || x >= W() || y >= H()) {
		return t_WATER;
	} else {
		return tiles_[x+(y*w_)]; 
	}
}


void
MapEditor::Resize(int w, int h)
{
	int old_w = w_, old_h = h_;
	w_ = w;
	h_ = h;
	enum Terrain* t = new enum Terrain[w_ * h_];
	for(int i=0; i<w_*h_; i++) {
		t[i] = t_GRASS;
	}
	for(int x=0; x<min(old_w, w_); x++) {
		for(int y=0; y<min(old_h, h_); y++) {
			t[x+(y*w_)] = tiles_[x+(y*old_w)];
		}
	}

	if(tiles_) {
		delete[] tiles_;
		tiles_ = nullptr;
	}
	tiles_ = t;
}


void 
MapEditor::SetTile(Point<int> tile, string idx)
{
	if(tile.x < 0 || tile.y < 0 || tile.x >= w_ || tile.y >= h_) {
		return; 
	}

	// check terrains
	static const struct { string idx; enum Terrain t; } terrains[] = {
		{ "grass_c", t_GRASS }, { "water_c", t_WATER },
		{ "desert_c", t_DESERT }, { "dirt_c", t_DIRT },
		{ "dirt2_c", t_DIRT2 }, { "snow_c", t_SNOW },
		{ "lava_c", t_LAVA }, { "tundra_c", t_TUNDRA },
		{ "lavarock_c", t_LAVAROCK }
	};
	for(auto const& terrain: terrains) {
		if(idx == terrain.idx) {
			tiles_[tile.x+(tile.y*w_)] = terrain.t;
		}
	}
}
