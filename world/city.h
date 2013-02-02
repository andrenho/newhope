#ifndef WORLD_CITY_H
#define WORLD_CITY_H

#include <string>
#include <vector>
#include "util/defines.h"
#include "util/point.h"
#include "util/rect.h"

enum CityStyle { VICTORIAN };

class Biome;
class Building;

class City {
public:
	City(Point<int> pos, const Biome& biome, CityStyle style);
	~City();

	const Rect Limits() const;
	std::string Layout(Point<int> p) const;
	const Building* BuildingInPoint(Point<int> p) const;
	inline void AddConnection(const City* c) { connections_.push_back(c); }

	// read members
	const Point<int> pos() const { return pos_; }
	const Biome& biome() const { return biome_; }
	CityStyle style() const { return style_; }
	const std::vector<const City*>& connections() const { return connections_; }
	const std::vector<const Building*>& buildings() const { return buildings_; }

private:
	const Point<int> pos_;
	const Biome& biome_;
	const CityStyle style_;
	std::vector<const City*> connections_;
	std::vector<const Building*> buildings_;

	DISALLOW_COPY_AND_ASSIGN(City);
};

#endif
