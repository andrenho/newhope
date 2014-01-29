// Copyright 2014 <Imperial Software>

#ifndef ENGINE_CITY_H_
#define ENGINE_CITY_H_

#include "engine/point.h"

class City {
public:
	City(Point pos) : Pos(pos) { }
	virtual ~City() { }

	const Point Pos;

private:
	City(const City&);
	City& operator=(const City&);
};

#endif  // ENGINE_CITY_H_
