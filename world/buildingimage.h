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
	BuildingImage(std::vector<std::string> layout, std::string sign="")
		: layout_(layout), sign_(sign) { }

	static const BuildingImage VICTORIAN_BANK, VICTORIAN_MARKET,
		     VICTORIAN_CAR_DEALERSHIP;

	// read members
	const int w() const { return layout_[0].size() / 2; }
	const int h() const { return layout_.size(); }
	const std::vector<std::string>& layout() const { return layout_; }
	const std::string sign() const { return sign_; }

private:
	const std::vector<std::string> layout_;
	const std::string sign_;
};

#endif
