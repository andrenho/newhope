#include "defines.h"

MapEditor::MapEditor()
{
	w_ = 50;
	h_ = 25;
}


enum Terrain 
MapEditor::Terrain(int x, int y) const 
{ 
	if(x < 0 || y < 0 || x >= W() || y >= H()) {
		return t_WATER;
	} else {
		return t_GRASS; 
	}
}


void
MapEditor::Resize(int w, int h)
{
	w_ = w;
	h_ = h;
}

