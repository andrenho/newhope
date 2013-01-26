#ifndef WORLD_BUILDING_H
#define WORLD_BUILDING_H

#include "util/defines.h"

class BuildingType;

class Building {
public:
	explicit Building(const BuildingType& type);

private:
	const BuildingType& type;

	DISALLOW_COPY_AND_ASSIGN(Building);
};

#endif
