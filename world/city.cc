#include "world/city.h"

#include "world/building.h"
#include "world/buildingtype.h"

City::City(Point<int> pos, const Biome& biome)
		: pos(pos), biome(biome)
{
	buildings.push_back(new Building(*this, BuildingType::BANK, -4, -4));
}


City::~City()
{
	for(auto& b: buildings)
		delete b;
	buildings.clear();
}


const Rect 
City::Limits() const
{
	Point<int> min_(pos), max_(pos);
	for(const auto& building: buildings) {
		if(pos.x + building->xrel < min_.x) {
			min_.x = pos.x + building->xrel;
		}
		if(pos.y + building->yrel < min_.y) {
			min_.y = pos.y + building->yrel;
		}
		if(pos.x + building->xrel + building->W() > max_.x) {
			max_.x = pos.x + building->xrel + building->W();
		}
		if(pos.y + building->yrel + building->H() > max_.y) {
			max_.y = pos.y + building->yrel + building->H();
		}
	}
	++max_.x; ++max_.y;
	return Rect(min_, max_);
}
