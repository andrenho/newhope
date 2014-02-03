#ifndef ENGINE_MAPGEN_H_
#define ENGINE_MAPGEN_H_

//#include <boost/polygon/polygon.hpp>
//#include <boost/polygon/voronoi.hpp>
#include <map>
#include <random>
#include <set>
#include <vector>

#include "engine/rectangle.h"
#include "engine/point.h"

struct PointData {
	PointData() : Biome(Block::GRASS) {}
	Block const* Biome;
};

class MapGen {
public:
	MapGen(int x1, int y1, int x2, int y2);
	MapGen(int x1, int y1, int x2, int y2, unsigned int seed);

	void Create();
	Block const* Terrain(int x, int y) const;

	virtual ~MapGen();

private:
	void CreatePoints(int points);
	void CreateQuadrants(int size);

	Point ClosestPoint(int x, int y) const;

	const Rectangle rect;
	//boost::polygon::voronoi_diagram<double> vd;
	std::default_random_engine generator;
	std::vector<Point> points;
	std::map<Point,PointData> data;

	std::map<const Rectangle,std::set<Point>> quadrants;

	mutable std::map<Point,Block const*> tile_cache;

	const int NUMPOINTS = 1000;
};

#endif  // ENGINE_MAPGEN_H_
