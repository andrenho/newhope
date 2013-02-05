#include "world/city.h"

using namespace std;

#include "util/logger.h"
#include "world/building.h"

City::City(Tile pos, const Biome& biome, CityStyle style)
		: pos_(pos), biome_(biome), style_(style)
{
	CreateLayout();
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
	Tile min_(pos_), max_(pos_);
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
City::Layout(Tile p) const
{
	if(!Limits().ContainsPoint(p)) {
		return "  ";
	}

	for(const auto& b: buildings_) {
		Tile p2 = p - pos_ - Tile(b->xrel(), b->yrel());
		return b->OutdoorsLayout(p2);
	}
	return "  ";
}


const Building* 
City::BuildingInPoint(Tile p) const
{
	for(const auto& b: buildings_) {
		if(b->Rectangle().ContainsPoint(p))
			return b;
	}
	return nullptr;
}



void
City::CreateLayout()
{
	int x1(-2), x2(2), turn(false);
	for(auto bt: BuildingList()) {
		int x(turn ? x1 : x2);
		const BuildingImage& image(Building::Image(*this, bt));
		if(turn) {
			x1 -= image.w() + 1;
			x = x1;
		}
		buildings_.push_back(new Building(*this, bt, x, -image.h()));
		if(!turn) {
			x2 += image.w() + 1;
		}
		turn = !turn;
	}
}


const vector<BuildingType>
City::BuildingList() const
{
	return { BANK, BANK };
}
