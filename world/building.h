#ifndef WORLD_BUILDING_H
#define WORLD_BUILDING_H

#include <string>

#include "util/defines.h"
#include "util/rect.h"
#include "world/buildingimage.h"
#include "world/city.h"

enum BuildingType { BANK };

class Building {
public:
	explicit Building(const City& city, const BuildingType type, 
			int xrel, int yrel);

	inline int X() const { return city_.pos().x + xrel_; }
	inline int Y() const { return city_.pos().y + yrel_; }
	inline int W() const { return image_.w(); }
	inline int H() const { return image_.h(); }
	inline const Rect Rectangle() const { 
		return Rect(X(), Y(), W()+1, H()+1); 
	}

	std::string OutdoorsLayout(int x, int y) const;
	std::string OutdoorsLayout(const Tile& p) const {
		return OutdoorsLayout(p.x, p.y);
	}

	int HeightAt(int x) const;
	inline const BuildingType Type() const { return type_; }

	// read members
	inline int xrel() const { return xrel_; }
	inline int yrel() const { return yrel_; }

private:
	const BuildingImage& Image() const;

	const int xrel_, yrel_;
	const City& city_;
	const BuildingType type_;
	const BuildingImage& image_;

	DISALLOW_COPY_AND_ASSIGN(Building);
};

#endif
