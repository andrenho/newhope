#ifndef WORLD_CITY_H
#define WORLD_CITY_H

#include <string>
#include <vector>
#include "util/defines.h"
#include "util/point.h"
#include "util/rect.h"

enum CityStyle { VICTORIAN };

class Biome;
class Building;

class City {
public:
	City(Point<int> pos, const Biome& biome, CityStyle style);
	~City();

	const Rect Limits() const;
	std::string Layout(Point<int> p) const;
	const Building* BuildingInPoint(Point<int> p) const;

	const Point<int> pos;
	const Biome& biome;
	const CityStyle Style;
	std::vector<const City*> connections;
	std::vector<const Building*> buildings;

private:
	DISALLOW_COPY_AND_ASSIGN(City);
};

#endif
