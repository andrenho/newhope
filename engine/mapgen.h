#ifndef ENGINE_MAPGEN_H_
#define ENGINE_MAPGEN_H_

#include <map>
#include <set>
#include <vector>

#include "engine/rectangle.h"
#include "engine/point.h"

struct PointData {
	PointData() : Biome(Block::GRASS), Altitude(0) {}
	Block const* Biome;
	int Altitude;
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
	void CreateHeightmap();
	void ApplyHeightmap();

	Point ClosestPoint(int x, int y) const;
	void RandomOffcentre(int& x, int& y, double& r) const;
	void CreateHill(int x, int y, double r);

	double Random() const;

	const Rectangle rect;
	std::vector<Point> points;
	std::map<Point,PointData> data;
	int hm[255][255]; // heightmap

	mutable std::map<Point,Block const*> tile_cache;

	const int NUMPOINTS = 1000;
};

#endif  // ENGINE_MAPGEN_H_
