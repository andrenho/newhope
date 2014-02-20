#ifndef ENGINE_CITY_H_
#define ENGINE_CITY_H_

#include <map>
#include <vector>
using namespace std;

#include "engine/building.h"
#include "engine/citylayout.h"
#include "engine/point.h"
#include "engine/resources.h"

class City final {
public:
    City(int x, int y, CityType type, int n);

    City(City&&) = default;
    City(const City&) = delete;
    City& operator=(const City&) = delete;

    int Tiles(Block (&block)[10], int x, int y) const;

    inline int W() const { return layout.w; }
    inline int H() const { return layout.h; }

    static CityType Type(Block biome);
    const vector<WorkerPair> Workers() const;

    const int X, Y;

private:
    CityLayout const& layout;
    vector<Building> buildings = {};
};

#endif  // ENGINE_CITY_H_


// vim: ts=4:sw=4:sts=4:expandtab
