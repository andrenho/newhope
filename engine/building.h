// Copyright 2014 <Imperial Software>

#ifndef ENGINE_BUILDING_H_
#define ENGINE_BUILDING_H_

#include "engine/buildinglayout.h"
#include "engine/block.h"
#include "engine/point.h"

class Building {
public:
	Building(BuildingType type, Point const& pos);
	virtual ~Building() { }

	int Tiles(Block (&block)[10], int x, int y);

	const Point Position;

private:
	Building(const Building&);
	Building& operator=(const Building&);

	const BuildingType type;
};

#endif  // ENGINE_BUILDING_H_
