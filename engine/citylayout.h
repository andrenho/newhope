// Copyright 2014 <Imperial Software>

#ifndef ENGINE_CITYLAYOUT_H_
#define ENGINE_CITYLAYOUT_H_

#include <map>
#include <string>
#include <vector>
#include <utility>

#include "engine/buildinglayout.h"
#include "engine/point.h"

enum CityType {
	AGRICULTURAL,
};

typedef std::pair<CityType, int> CityPair;
typedef std::pair<BuildingPair, Point> BuildingPosition;

struct CityLayout {
	int w, h;
	std::vector<BuildingPosition> buildings;
};

extern std::map<CityPair, CityLayout> CityLayouts;

#endif  // ENGINE_CITYLAYOUT_H_
