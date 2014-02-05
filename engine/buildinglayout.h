#ifndef ENGINE_BUILDINGLAYOUT_H_
#define ENGINE_BUILDINGLAYOUT_H_

#include <map>
#include <string>
#include <vector>
#include <utility>

#include "engine/block.h"

enum BuildingType {
	GENERAL_STORE,
};

typedef std::pair<BuildingType, int> BuildingPair;

struct BuildingLayout {
	BuildingLayout()
		: w(0), h(0), floors(0), floor({}), label({}) {}

	BuildingLayout(int w, int h, int floors,
			std::vector<std::vector<std::string>> floor,
			std::map<const char, const Block*> label)
		: w(w), h(h), floors(floors), floor(floor), label(label) {}

	int w, h, floors;
	std::vector<std::vector<std::string>> floor;
	std::map<const char, const Block*> label;
};

extern std::map<BuildingPair, BuildingLayout> BuildingLayouts;

#endif  // ENGINE_BUILDINGLAYOUT_H_
