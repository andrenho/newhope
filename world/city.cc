#include "world/city.h"

#include "world/building.h"

City::City(Point<int> pos, const Biome& biome, CityStyle style)
		: pos_(pos), biome_(biome), style_(style)
{
	buildings_.push_back(new Building(*this, BuildingType::BANK, -4, -4));
}


City::~City()
{
	for(auto& b: buildings_)
		delete b;
	buildings_.clear();
}


const Rect 
City::Limits() const
{
	Point<int> min_(pos_), max_(pos_);
	for(const auto& building: buildings_) {
		if(building->X() < min_.x) {
			min_.x = building->X();
		}
		if(building->Y() < min_.y) {
			min_.y = building->Y();
		}
		if(building->X() + building->W() > max_.x + 1) {
			max_.x = building->X() + building->W() + 1;
		}
		if(building->Y() + building->H() > max_.y + 1) {
			max_.y = building->Y() + building->H() + 1;
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

	for(const auto& b: buildings_) {
		Point<int> p2 = p - pos_ - Point<int>(b->xrel(), b->yrel());
		return b->OutdoorsLayout(p2);
	}
	return "  ";
}


const Building* 
City::BuildingInPoint(Point<int> p) const
{
	for(const auto& b: buildings_) {
		if(b->Rectangle().ContainsPoint(p))
			return b;
	}
	return nullptr;
}
