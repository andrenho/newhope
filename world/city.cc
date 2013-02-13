#include "world/city.h"

using namespace std;

#include "util/logger.h"
#include "world/building.h"

City::City(Tile pos, const Biome& biome, CityStyle style)
		: pos_(pos), biome_(biome), style_(style)
{
	CreateLayout();

	logger.Debug("City limits:");
	Limits().Debug();

	for(auto& b: buildings_) {
		logger.Debug("Building limits:");
		b->Limits().Debug();
	}
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
		if(building->x() < min_.x) {
			min_.x = building->x();
		}
		if(building->y() < min_.y) {
			min_.y = building->y();
		}
		if(building->x() + building->w() > max_.x) {
			max_.x = building->x() + building->w();
		}
		if(building->y() + building->h() > max_.y) {
			max_.y = building->y() + building->h();
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
		string s = b->OutdoorsLayout(p2);
		if(s != "  ") {
			return s;
		}
	}
	return "  ";
}


const Building* 
City::BuildingInPoint(Tile p) const
{
	for(const auto& b: buildings_) {
		if(b->Limits().ContainsPoint(p)) {
			return b;
		}
	}
	return nullptr;
}



void
City::CreateLayout()
{
	int x1(-2), x2(2), turn(false);
	for(auto bt: BuildingList()) {
		auto b = new Building(*this, bt, 0, 0);
		int x(turn ? x1 : x2);
		if(turn) {
			x1 -= b->w() + 1;
			x = x1;
		}
		b->Move(x, -b->h());
		if(!turn) {
			x2 += b->w() + 1;
		}
		turn = !turn;
		buildings_.push_back(b);
	}
}


const vector<BuildingType>
City::BuildingList() const
{
	return { CAR_DEALERSHIP, BANK, MARKET };
}
