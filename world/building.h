#ifndef WORLD_BUILDING_H
#define WORLD_BUILDING_H

#include <string>

#include "util/defines.h"
#include "util/rect.h"
#include "world/buildingimage.h"

class City;

enum BuildingType { BANK, MARKET, CAR_DEALERSHIP };

class Building {
public:
	explicit Building(const City& city, const BuildingType type, 
			int xrel, int yrel);

	const Rect Limits() const { 
		return Rect(x(), y(), w(), h()); 
	}

	std::string OutdoorsLayout(int x, int y) const;
	std::string OutdoorsLayout(const Tile& p) const {
		return OutdoorsLayout(p.x, p.y);
	}

	int HeightAt(int x) const;
	int x() const;
	int y() const;
	int w() const;
	int h() const;
	const Rect ParkingLot() const; 
	void Move(int x, int y) { xrel_ += x; yrel_ += y; }

	const BuildingType type() const { return type_; }
	const int uniqueID() const { return unique_id_; }

	static const BuildingImage& Image(const City& city, 
			const BuildingType type);
	const BuildingImage& Image() const {
		return Building::Image(city_, type_);
	}


	// read members
	inline int xrel() const { return xrel_; }
	inline int yrel() const { return yrel_; }

	static int unique_id_counter_;

private:
	int xrel_, yrel_;
	const City& city_;
	const BuildingType type_;
	const BuildingImage& image_;
	int unique_id_;
	Rect patio_;

	DISALLOW_COPY_AND_ASSIGN(Building);
};

#endif
