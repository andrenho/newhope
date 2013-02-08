#include "world/building.h"

#include "world/city.h"

#include <cstdlib>
using namespace std;

int Building::unique_id_counter_ = 0;

Building::Building(const City& city, const BuildingType type, 
		int xrel, int yrel)
	: xrel_(xrel), yrel_(yrel), city_(city), type_(type), 
	  image_(Image()), unique_id_(unique_id_counter_++),
	  patio_(Rect(0,0,0,0)), door_(Tile(-1,-1))
{
	// add patio to dealerships
	if(type == BuildingType::CAR_DEALERSHIP) {
		patio_ = Rect(0, 3, 15, 10);
	}

	// find door
	int j = 0;
	for(const auto& s: image_.layout()) {
		for(int i=0; i<s.size(); i+=2) {
			if(s[i] == 'd' && s[i+1] == 's') {
				door_ = Tile(x()+(i/2), y()+j);
				goto done;
			}
		}
		++j;
	}
done:;
}


int Building::x() const { return city_.pos().x + xrel_; }
int Building::y() const { return city_.pos().y + yrel_; }
int Building::w() const { return image_.w() - patio_.x + patio_.w; }
int Building::h() const { return image_.h() - patio_.y + patio_.h; }


string 
Building::OutdoorsLayout(int x, int y) const
{
	if(x < 0 || y < 0 || x >= image_.w() || y >= image_.h()) {
		return "  ";
	} else {
		return image_.layout()[y].substr(x*2, 2);
	}
}


int 
Building::HeightAt(int x) const
{
	x -= city_.pos().x + xrel_;
	for(int y=0; y < h(); y++) {
		if(OutdoorsLayout(x, y) == "  ") {
			return y;
		}
	}
	return h();
}


const BuildingImage& 
Building::Image(const City& city, const BuildingType type)
{
	if(city.style() == CityStyle::VICTORIAN) {
		if(type == BuildingType::BANK) {
			return BuildingImage::VICTORIAN_BANK;
		} else if(type == BuildingType::MARKET) {
			return BuildingImage::VICTORIAN_MARKET;
		} else if(type == BuildingType::CAR_DEALERSHIP) {
			return BuildingImage::VICTORIAN_CAR_DEALERSHIP;
		} else {
			abort();
		}
	} else {
		abort();
	}
}


const Rect
Building::ParkingLot() const
{
	return patio_ + Tile(x(), y());
}


void 
Building::Move(int x, int y) 
{ 
	xrel_ += x; yrel_ += y;
	door_ += Tile(x, y);
}
