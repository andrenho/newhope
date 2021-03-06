#ifndef ENGINE_WORLD_H_
#define ENGINE_WORLD_H_

#include <map>
#include <memory>
#include <vector>
using namespace std;

#include "engine/block.h"
#include "engine/city.h"
#include "engine/mapgen/mapgen.h"
#include "engine/object.h"

class Object;

class World final {
public:
    World(int x1, int y1, int x2, int y2, unsigned int seedp);
    ~World();

    World(const World&) = delete;
    World& operator=(const World&) = delete;

    void Initialize();
    void Step();

    int Tiles(Block (&block)[10], int x, int y) const;
    bool IsTileWalkable(int x, int y) const;
    void Limits(int& x1, int& y1, int& x2, int& y2) const;

    inline class Hero& Hero() const { return *hero; }
    inline vector<Object::Ptr> const& Objects() const { return objects; }
    inline vector<City> const& Cities() const { return cities; }
    inline vector<vector<Point>> const& Rivers() const { return mapgen.Rivers(); }
    inline vector<vector<Point>> const& Roads() const { return mapgen.Roads(); }

    void RecalculatePrices();

    double Random() const;

    const BlockManager Blocks;

private:
    void CreateCities();
    void AddObject(Object::Ptr obj);
    void AddWorkers();
    void AddStaticObjects();
    void AddStaticObject(double x, double y, double w, double h);

    static void FreeStaticShape(struct cpBody *body, struct cpShape *shape, void* data);
    static int CollisionCallback(struct cpArbiter *arb, struct cpSpace *space, void *data);

    int x1, y1, x2, y2;
    mutable unsigned int seedp;
    shared_ptr<class Hero> hero = nullptr;

    class MapGen mapgen;

    vector<Object::Ptr> objects = {};
    vector<City> cities = {};

    map<struct cpBody*, Object::Ptr> physics_ptr = {};
};

#endif  // ENGINE_WORLD_H_


// vim: ts=4:sw=4:sts=4:expandtab
