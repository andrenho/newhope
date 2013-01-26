#ifndef WORLD_CITY_H
#define WORLD_CITY_H

#include <vector>
#include "util/defines.h"
#include "util/point.h"

class Biome;
class Building;

class City {
public:
	City(Point<int> pos, const Biome& biome);
	~City();

	const Point<int> pos;
	const Biome& biome;
	std::vector<const City*> connections;
	std::vector<const Building*> buildings;

private:
	DISALLOW_COPY_AND_ASSIGN(City);
};

#endif
