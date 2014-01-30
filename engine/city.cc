// Copyright 2014 <Imperial Software>

#include "engine/city.h"

City::City(Point const& pos, CityType type, int n)
	: Position(pos), layout(CityLayouts[CityPair(type, n)])
{
	for(auto const& bpos : layout.buildings) {
		const Building* b = new Building(bpos.second, 
				bpos.first.first, bpos.first.second);
		buildings.push_back(b);
	}
}


City::~City() 
{
	for(auto& building : buildings) {
		delete building;
	}
}


int 
City::Tiles(Block* (&block)[10], int x, int y) const
{
	for(auto const& b : buildings) {
		if(x >= b->Position.X() && x < b->Position.X()+b->W()
		&& y >= b->Position.Y() && y < b->Position.Y()+b->H()) {
			return b->Tiles(block, x, y);
		}
	}
	return 0;
}
