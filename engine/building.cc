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
		unsigned int ui = static_cast<unsigned int>(i),
		             uy = static_cast<unsigned int>(y),
		             ux = static_cast<unsigned int>(x);
		const char blk = layout.floor[ui][uy][ux];
		block[i] = layout.label.at(blk);
	}
	return layout.floors;
}
