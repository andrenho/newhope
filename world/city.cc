#include "world/city.h"

#include "world/building.h"

City::City(Point<int> pos, const Biome& biome, CityStyle style)
		: pos(pos), biome(biome), Style(style)
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
		if(pos.x + building->xrel + building->W() > max_.x + 1) {
			max_.x = pos.x + building->xrel + building->W() + 1;
		}
		if(pos.y + building->yrel + building->H() > max_.y + 1) {
			max_.y = pos.y + building->yrel + building->H() + 1;
		}
	}
	++max_.x; ++max_.y;
	return Rect(min_, max_);
}


std::string 
City::Layout(Point<int> p) const
{
	if(!Limits().ContainsPoint(p)) {
		return "  ";
	}

	for(const auto& b: buildings) {
		Point<int> p2 = p - pos - Point<int>(b->xrel, b->yrel);
		return b->OutdoorsLayout(p2);
	}
	return "  ";
}


const Building* 
City::BuildingInPoint(Point<int> p) const
{
	for(const auto& b: buildings) {
		if(b->Rectangle().ContainsPoint(p))
			return b;
	}
	return nullptr;
}
