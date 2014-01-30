// Copyright 2014 <Imperial Software>

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
	int w, h, floors;
	std::vector<std::vector<std::string>> floor;
	std::map<const char, Block*> label;
};

extern std::map<BuildingPair, BuildingLayout> BuildingLayouts;

#endif  // ENGINE_BUILDINGLAYOUT_H_
