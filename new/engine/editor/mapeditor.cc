#include "defines.h"

#include <algorithm>

MapEditor::MapEditor()
	: tiles_(nullptr)
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
	if(tiles_) {
		delete[] tiles_;
		tiles_ = nullptr;
	}

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
	tiles_ = t;
}
