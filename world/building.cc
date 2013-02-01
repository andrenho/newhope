#include "world/building.h"

#include <cstdlib>
using namespace std;

Building::Building(const City& city, const BuildingType type, 
		int xrel, int yrel)
	: xrel(xrel), yrel(yrel), city(city), type(type), 
	  image(Image())
{

}


string 
Building::OutdoorsLayout(int x, int y) const
{
	if(x < 0 || y < 0 || x >= W() || y >= H()) {
		return "  ";
	} else {
		return image.layout[y].substr(x*2, 2);
	}
}


int 
Building::HeightAt(int x) const
{
	x -= city.pos.x + xrel;
	for(int y=0; y < H(); y++) {
		if(OutdoorsLayout(x, y) == "  ") {
			return y;
		}
	}
	return H();
}


const BuildingImage& 
Building::Image() const
{
	if(city.Style == CityStyle::VICTORIAN) {
		if(type == BuildingType::BANK) {
			return BuildingImage::VICTORIAN_BANK;
		} else {
			abort();
		}
	} else {
		abort();
	}
}
