// Copyright 2014 <Imperial Software>

#ifndef ENGINE_BUILDINGLAYOUT_H_
#define ENGINE_BUILDINGLAYOUT_H_

#include <map>
#include <string>
#include <vector>

#include "engine/block.h"

enum BuildingType {
	GENERAL_STORE,
};

struct BuildingLayout {
	int w, h, height;
	BuildingType type;
	std::vector<std::vector<std::string>> floor;
	std::map<char, Block> label;
};

extern BuildingLayout BuildingLayouts[];

#endif  // ENGINE_BUILDINGLAYOUT_H_
