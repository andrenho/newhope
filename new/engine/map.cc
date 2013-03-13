#include "defines.h"

Map::Map()
{
	w_ = h_ = 5000;
}


enum Terrain 
Map::Terrain(int x, int y) const 
{ 
	if(x < 0 || y < 0 || x >= W() || y >= H()) {
		return t_WATER;
	} else {
		return t_GRASS;
	}
}
