#include "world/city.h"

#include "world/building.h"
#include "world/buildingtype.h"

City::City(Point<int> pos, const Biome& biome)
		: pos(pos), biome(biome)
{
	buildings.push_back(new Building(BuildingType::BANK, 5, 5));
}


City::~City()
{
}
