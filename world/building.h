#ifndef WORLD_BUILDING_H
#define WORLD_BUILDING_H

#include <string>

#include "util/defines.h"
#include "util/rect.h"
#include "world/buildingimage.h"

class City;

enum BuildingType { BANK };

class Building {
public:
	explicit Building(const City& city, const BuildingType type, 
			int xrel, int yrel);

	inline const Rect Rectangle() const { 
		return Rect(X(), Y(), W()+1, H()+1); 
	}

	std::string OutdoorsLayout(int x, int y) const;
	std::string OutdoorsLayout(const Tile& p) const {
		return OutdoorsLayout(p.x, p.y);
	}

	int HeightAt(int x) const;
	int X() const;
	int Y() const;
	int W() const;
	int H() const;

	const BuildingType Type() const { return type_; }
	const int UniqueID() const { return unique_id_; }

	static const BuildingImage& Image(const City& city, 
			const BuildingType type);

	// read members
	inline int xrel() const { return xrel_; }
	inline int yrel() const { return yrel_; }

	static int unique_id_counter_;

private:
	const BuildingImage& Image() const {
		return Building::Image(city_, type_);
	}

	const int xrel_, yrel_;
	const City& city_;
	const BuildingType type_;
	const BuildingImage& image_;

	int unique_id_;

	DISALLOW_COPY_AND_ASSIGN(Building);
};

#endif
