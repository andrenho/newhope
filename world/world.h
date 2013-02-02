#ifndef WORLD_WORLD_H
#define WORLD_WORLD_H

#include <set>
#include <vector>

#include "util/defines.h"
#include "util/point.h"
#include "util/mapcache.h"

class MapBuild;
class Person;

// the order is the importance
typedef enum {
	t_WATER		= 99,
	t_LAVA          = 98,
	t_SNOW          = 80,
	t_GRASS		= 50,
	t_LAVAROCK      = 5,
	t_DIRT          = 20,
	t_EARTH,
	t_ROCK,
	t_TUNDRA,
	t_HOTFOREST,
	t_COLDFOREST,

	t_RIVER,
	t_ROAD,

} TerrainType;

typedef enum { NO_TREE = 0, TREE_ROUND, TREE_POINTY } TreeType;

class World {
public:
	World(int w, int h);
	~World();

	void Process();
	TerrainType Terrain(Point<int> p, bool use_cache=true) const;
	int Special(Point<int> p) const;
	TreeType Tree(Point<int> p) const;
	bool TreeSmall(Point<int> p) const;

	static TerrainType TerrainCache(void* obj, Point<int> p);

	inline Person& Hero() const { return *people_[0]; }

	inline const int w() const { return w_; }
	inline const int h() const { return h_; }
	inline const MapBuild& map() const { return *map_; }
	inline const std::vector<Person*> people() const { return people_; }

private:
	void CreatePathsCache();
	void AddPoints(Point<int> p1, Point<int> p2, 
			std::set<Point<int>>& points, int w);

	static TerrainType FindBiome(World* ths, Point<int> p);

	std::vector<Point<int>> riverpts, roadpts, lavapts;
	mapcache<Point<int>,TerrainType>* cache;
	int randspecial[100][100];

	const int w_, h_;
	const MapBuild* map_;
	std::vector<Person*> people_;

	DISALLOW_COPY_AND_ASSIGN(World);
};

#endif
