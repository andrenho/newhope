// Copyright 2014 <Imperial Software>

#ifndef ENGINE_BUILDING_H_
#define ENGINE_BUILDING_H_

#include "engine/point.h"

class Building {
public:
	Building() { }
	virtual ~Building() { }

private:
	Building(const Building&);
	Building& operator=(const Building&);

};

#endif  // ENGINE_BUILDING_H_
