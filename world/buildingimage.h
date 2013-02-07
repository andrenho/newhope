#ifndef WORLD_BUILDINGIMAGE_H
#define WORLD_BUILDINGIMAGE_H

#include <string>
#include <vector>

// TODO building types:
//  - bank
//  - car dealership
//  - market
//  - dwellings

// Second phase:
//  - merchants guild (?)
//  - knights guild (?)
//  - bar

class BuildingImage {
public:
	BuildingImage(int w, int h, std::vector<std::string> layout)
		: w_(w), h_(h), layout_(layout) { }

	static const BuildingImage VICTORIAN_BANK;

	// read members
	const int w() const { return w_; }
	const int h() const { return h_; }
	const std::vector<std::string>& layout() const { return layout_; }

private:
	const int w_, h_;
	const std::vector<std::string> layout_;
};

#endif
