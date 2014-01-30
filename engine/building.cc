// Copyright 2014 <Imperial Software>

#include "engine/building.h"

Building::Building(Point const& pos, BuildingType type, int n)
	: Position(pos), type(type), 
	  layout(BuildingLayouts[BuildingPair(type, n)])
{
}


int 
Building::Tiles(Block* (&block)[10], int x, int y) const
{
	for(int i=0; i<layout.floors; i++) {
		const char blk = layout.floor[i][y][x];
		block[i] = layout.label.at(blk);
	}
	return layout.floors;
}
