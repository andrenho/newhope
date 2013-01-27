#include "world/building.h"

#include "world/buildingtype.h"

Building::Building(const BuildingType& type, int xrel, int yrel)
	: xrel(xrel), yrel(yrel), type(type)
{

}
