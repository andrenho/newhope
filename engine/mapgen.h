#ifndef ENGINE_MAPGEN_H_
#define ENGINE_MAPGEN_H_

#include <map>
#include <set>
#include <vector>

#include "engine/rectangle.h"
#include "engine/point.h"

struct PointData final {
	PointData() : Biome(Block::GRASS), Altitude(0), Moisture(0) {}
	Block const* Biome;
	double Altitude;  // between 0 and 1
	double Moisture;
};

class MapGen final {
public:
	MapGen(int x1, int y1, int x2, int y2);
	MapGen(int x1, int y1, int x2, int y2, unsigned int seed);

	void Create();
	Block const* Terrain(int x, int y) const;

	inline std::vector<std::vector<Point>> const& Rivers() const { return rivers; }

	~MapGen();

private:
	void CreatePoints(int points);
	void CreateHeightmap();
	void CalculateMoisture();
	void CreateBiomes();

	Point ClosestPoint(int x, int y) const;
	void RandomOffcentre(int& x, int& y, double& r) const;
	void CreateHill(int x, int y, double r);
	double DistanceFromWater(Point const& p) const;

	double PointAltitude(Point const& p) const;
	double Random() const;
	Point RandomPoint() const;

	const Rectangle rect;
	mutable unsigned int seedp;

	std::vector<Point> points;
	std::map<Point,PointData> data;
	std::vector<std::vector<Point>> rivers;
	double hm[255][255]; // heightmap

	mutable std::map<Point,Block const*> tile_cache;

	const int NUMPOINTS = 1000;
};

#endif  // ENGINE_MAPGEN_H_
