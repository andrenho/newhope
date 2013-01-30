#include "world/building.h"

#include <cstdlib>
using namespace std;

Building::Building(const City& city, const BuildingType& type, 
		int xrel, int yrel)
	: xrel(xrel), yrel(yrel), city(city), type(type)
{

}


string 
Building::OutdoorsLayout(int x, int y) const
{
	if(x < 0 || y < 0 || x >= W() || y >= H()) {
		return "  ";
	} else {
		return type.layout[y].substr(x*2, 2);
	}
}
