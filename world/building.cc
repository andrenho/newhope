#include "world/building.h"

#include "world/city.h"

#include <cstdlib>
using namespace std;

int Building::unique_id_counter_ = 0;

Building::Building(const City& city, const BuildingType type, 
		int xrel, int yrel)
	: xrel_(xrel), yrel_(yrel), city_(city), type_(type), 
	  image_(Image()), unique_id_(unique_id_counter_++)
{
}


int Building::X() const { return city_.pos().x + xrel_; }
int Building::Y() const { return city_.pos().y + yrel_; }
int Building::W() const { return image_.w(); }
int Building::H() const { return image_.h(); }

string 
Building::OutdoorsLayout(int x, int y) const
{
	if(x < 0 || y < 0 || x >= W() || y >= H()) {
		return "  ";
	} else {
		return image_.layout()[y].substr(x*2, 2);
	}
}


int 
Building::HeightAt(int x) const
{
	x -= city_.pos().x + xrel_;
	for(int y=0; y < H(); y++) {
		if(OutdoorsLayout(x, y) == "  ") {
			return y;
		}
	}
	return H();
}


const BuildingImage& 
Building::Image(const City& city, const BuildingType type)
{
	if(city.style() == CityStyle::VICTORIAN) {
		if(type == BuildingType::BANK) {
			return BuildingImage::VICTORIAN_BANK;
		} else if(type == BuildingType::MARKET) {
			return BuildingImage::VICTORIAN_MARKET;
		} else {
			abort();
		}
	} else {
		abort();
	}
}
