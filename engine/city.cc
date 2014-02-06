#include "engine/city.h"

#include "./globals.h"
#include "engine/world.h"

City::City(int x, int y, CityType type, int n)
	: X(x), Y(y), layout(CityLayouts[CityPair(type, n)]),
	  buildings(std::vector<const Building*>())
{
	for(auto const& bpos : layout.buildings) {
		const Building* b = new Building(
				static_cast<int>(bpos.second.X()), 
				static_cast<int>(bpos.second.Y()), 
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
City::Tiles(const Block* (&block)[10], int x, int y) const
{
	for(auto const& b : buildings) {
		if(x >= b->X && x < b->X+b->W()
		&& y >= b->Y && y < b->Y+b->H()) {
			return b->Tiles(block, x, y);
		}
	}
	block[0] = Block::EMPTY;
	return 1;
}


CityType 
City::Type(Block const* b)
{
	if(b == Block::GRASS) {
		return CityType::AGRICULTURAL;
	} else if(b == Block::SNOW) {
		return CityType::REFINERY;
	} else if(b == Block::BARE) {
		return CityType::MINING;
	} else if(b == Block::TEMPERATE_FOREST || b == Block::TROPICAL_FOREST) {
		return CityType::FORESTAL;
	} else if(b == Block::DESERT || b == Block::BEACH) {
		return CityType::CHEMICAL;
	} else {
		return static_cast<CityType>(world->Random() * CityType::__COUNT__);
	}
}
