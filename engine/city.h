// Copyright 2014 <Imperial Software>

#ifndef ENGINE_CITY_H_
#define ENGINE_CITY_H_

#include <vector>

#include "engine/building.h"
#include "engine/citylayout.h"
#include "engine/point.h"

class City {
public:
	City(Point const& pos, CityType type, int n);
	virtual ~City();

	int Tiles(Block* (&block)[10], int x, int y) const;

	inline int W() const { return layout.w; }
	inline int H() const { return layout.h; }

	const Point Position;

private:
	City(const City&);
	City& operator=(const City&);

	CityLayout const& layout;
	std::vector<const Building*> buildings;
};

#endif  // ENGINE_CITY_H_
