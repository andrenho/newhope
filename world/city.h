#ifndef WORLD_CITY_H
#define WORLD_CITY_H

#include <string>
#include <vector>
#include "util/defines.h"
#include "util/point.h"
#include "util/rect.h"
#include "world/building.h"

enum CityStyle { VICTORIAN };

class Biome;
class Building;

class City {
public:
	City(Tile pos, const Biome& biome, CityStyle style);
	~City();

	const Rect Limits() const;
	std::string Layout(Tile p) const;
	const Building* BuildingInPoint(Tile p) const;
	inline void AddConnection(const City* c) { connections_.push_back(c); }

	// read members
	const Tile pos() const { return pos_; }
	const Biome& biome() const { return biome_; }
	const CityStyle style() const { return style_; }
	const std::vector<const City*>& connections() const { return connections_; }
	const std::vector<const Building*>& buildings() const { return buildings_; }

private:
	void CreateLayout();
	const std::vector<BuildingType> BuildingList() const;

	const Tile pos_;
	const Biome& biome_;
	const CityStyle style_;
	std::vector<const City*> connections_;
	std::vector<const Building*> buildings_;
	std::vector<Tile> main_street_;

	DISALLOW_COPY_AND_ASSIGN(City);
};

#endif
