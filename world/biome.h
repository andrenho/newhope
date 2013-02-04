#ifndef BIOME_H
#define BIOME_H

#include "util/polygon.h"
#include "world/world.h"

class Biome {
public:
	Biome(Polygon<TileElevation>* polygon)
		: polygon_(polygon), terrain_(t_DIRT), elevation_(0), 
		  moisture_(0), has_city_(false) {}
	~Biome() { delete polygon_; }

	// read members
	inline Polygon<TileElevation>& polygon() const { return *polygon_; }
	inline const TerrainType& terrain() const { return terrain_; }
	inline const int elevation() const { return elevation_; }
	inline const int moisture() const { return moisture_; }
	inline const bool has_city() const { return has_city_; }

	// write members
	inline void set_terrain(TerrainType t) { terrain_ = t; }
	inline void set_elevation(int e) { elevation_ = e; }
	inline void set_moisture(int m) { moisture_ = m; }
	inline bool set_has_city(bool b) { return has_city_ = b; }

private:
	Polygon<TileElevation>* polygon_;
	TerrainType terrain_;
	int elevation_;
	int moisture_;
	bool has_city_;
};

#endif
