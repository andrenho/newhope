#ifndef WORLD_BUILDING_H
#define WORLD_BUILDING_H

#include "util/defines.h"

class BuildingType;

class Building {
public:
	explicit Building(const BuildingType& type, int xrel, int yrel);

private:
	const BuildingType& type;
	const int xrel, yrel;

	DISALLOW_COPY_AND_ASSIGN(Building);
};

#endif
