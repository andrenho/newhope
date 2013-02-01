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

	inline int X() const { return city.pos.x + xrel; }
	inline int Y() const { return city.pos.y + yrel; }
	inline int W() const { return image.w; }
	inline int H() const { return image.h; }
	inline const Rect Rectangle() const { 
		return Rect(X(), Y(), W()+1, H()+1); 
	}

	std::string OutdoorsLayout(int x, int y) const;
	template <typename T> std::string OutdoorsLayout(Point<T> p) const {
		return OutdoorsLayout(p.x, p.y);
	}

	int HeightAt(int x) const;
	inline const BuildingType Type() const { return type; }

	const int xrel, yrel;
private:
	const BuildingImage& Image() const;

	const City& city;
	const BuildingType type;
	const BuildingImage& image;

	DISALLOW_COPY_AND_ASSIGN(Building);
};

#endif
