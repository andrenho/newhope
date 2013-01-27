#ifndef WORLD_BUILDING_H
#define WORLD_BUILDING_H

#include "util/defines.h"
#include "world/buildingtype.h"

class BuildingType;

class Building {
public:
	explicit Building(const BuildingType& type, int xrel, int yrel);

	inline int W() const { return type.w; }
	inline int H() const { return type.h; }

	const int xrel, yrel;
private:
	const BuildingType& type;

	DISALLOW_COPY_AND_ASSIGN(Building);
};

#endif
