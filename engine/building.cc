#include "engine/building.h"

Building::Building(int x, int y, BuildingType type, int n)
	: X(x), Y(y), type(type), 
	  layout(BuildingLayouts[BuildingPair(type, n)])
{
}


int 
Building::Tiles(const Block* (&block)[10], int x, int y) const
{
	for(int i=0; i<layout.floors; i++) {
		const char blk = layout.floor[i][y][x];
		block[i] = layout.label.at(blk);
	}
	return layout.floors;
}
