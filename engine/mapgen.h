#ifndef ENGINE_MAPGEN_H_
#define ENGINE_MAPGEN_H_

#include <unordered_map>
#include <unordered_set>
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
	std::unordered_set<Point> CitiesPositions(unsigned int n) const;

	inline std::vector<std::vector<Point>> const& Rivers() const { return rivers; }

	~MapGen();

private:
	void CreatePoints(int points);
	void CreateHeightmap();
	void CalculateMoisture();
	void CreateBiomes();
	void CreateBeaches();

	void AddRiverTiles();
	void PlotRiverCircle(int x, int y, int r);
	void AddRiverTile(int x, int y);

	Point ClosestPoint(int x, int y) const;
	void RandomOffcentre(int& x, int& y, double& r) const;
	void CreateHill(int x, int y, double r);
	double DistanceFromWater(Point const& p) const;

	double PointAltitude(Point const& p) const;
	Point RandomPoint() const;
	bool RandomPointWithBiome(Point& p, Block const* biome,
		std::unordered_set<Point> ignore) const;

	const Rectangle rect;
	mutable unsigned int seedp;

	std::unordered_set<Point> points;
	std::unordered_map<Point,PointData> data;
	std::vector<std::vector<Point>> rivers;
	double hm[255][255]; // heightmap

	mutable std::unordered_map<Point,Block const*> tile_cache;
	std::unordered_set<Point> river_tiles;

	const int NUMPOINTS = 1250;
};

#endif  // ENGINE_MAPGEN_H_
