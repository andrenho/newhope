#ifndef ENGINE_MAPGEN_MAPGEN_H_
#define ENGINE_MAPGEN_MAPGEN_H_

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

#include "engine/block.h"
#include "engine/rectangle.h"
#include "engine/point.h"
#include "engine/mapgen/rivergen.h"
#include "engine/mapgen/roadgen.h"

struct PointData final {
    PointData() : Biome(Block::GRASS), Altitude(0), Moisture(0) {}
    Block Biome;
    double Altitude;  // between 0 and 1
    double Moisture;
};

class MapGen final {
public:
    MapGen(int x1, int y1, int x2, int y2);
    MapGen(int x1, int y1, int x2, int y2, unsigned int seed);

    MapGen(MapGen&&) = default;
    MapGen(const MapGen&) = delete;
    MapGen& operator=(const MapGen&) = delete;

    void Create();
    void CreateRoads(vector<class City>& cities);
    
    Block Terrain(int x, int y) const;
    unordered_set<Point> CitiesPositions(unsigned int n) const;

    inline vector<vector<Point>> const& Rivers() const { return rivers; }
    inline vector<vector<Point>> const& Roads() const { return roads; }

    ~MapGen();

private:
    void CreatePoints(int points);
    void CreateHeightmap();
    void CalculateMoisture();
    void CreateBiomes();
    void CreateBeaches();

    Point ClosestPoint(int x, int y) const;
    static void RandomOffcentre(int& x, int& y, double& r);
    void CreateHill(int x, int y, double r);
    double DistanceFromWater(Point const& p) const;

    double PointAltitude(Point const& p) const;
    bool RandomPointWithBiome(Point& p, Block biome,
        unordered_set<Point> ignore) const;

    const Rectangle_ rect;
    mutable unsigned int seedp;

    unordered_set<Point> points = {};
    unordered_map<Point,PointData> data = {};
    vector<vector<Point>> rivers = {}, roads = {};
    double hm[255][255] = {}; // heightmap

    mutable unordered_map<Point,Block> tile_cache = {};

    unique_ptr<class Rivergen> rivergen = nullptr;
    unique_ptr<class Roadgen> roadgen = nullptr;

    const int NUMPOINTS = 1250;
};

#endif  // ENGINE_MAPGEN_MAPGEN_H_


// vim: ts=4:sw=4:sts=4:expandtab
