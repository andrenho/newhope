#ifndef WORLD_BUILDINGTYPE_H
#define WORLD_BUILDINGTYPE_H

#include <string>
#include <vector>

class BuildingType {
public:
	BuildingType(int w, int h, std::vector<std::string> layout)
		: w(w), h(h), layout(layout) { }

	static const BuildingType BANK;

	const int w, h;
	const std::vector<std::string> layout;
};

#endif
