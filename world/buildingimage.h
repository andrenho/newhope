#ifndef WORLD_BUILDINGIMAGE_H
#define WORLD_BUILDINGIMAGE_H

#include <string>
#include <vector>

class BuildingImage {
public:
	BuildingImage(int w, int h, std::vector<std::string> layout)
		: w(w), h(h), layout(layout) { }

	static const BuildingImage VICTORIAN_BANK;

	const int w, h;
	const std::vector<std::string> layout;
};

#endif
