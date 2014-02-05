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

struct CityLayout final {
	CityLayout(int w, int h, 
		   std::vector<BuildingPosition> buildings)
		: w(w), h(h), buildings(buildings) {}

	CityLayout()
		: w(0), h(0), buildings({}) {}

	const int w, h;
	std::vector<BuildingPosition> buildings;
};

extern std::map<CityPair, CityLayout> CityLayouts;

#endif  // ENGINE_CITYLAYOUT_H_
