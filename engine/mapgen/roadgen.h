#ifndef ENGINE_MAPGEN_ROADGEN_H_
#define ENGINE_MAPGEN_ROADGEN_H_

#include <set>
#include <vector>
using namespace std;

#include "engine/mapgen/seggen.h"

class Roadgen final : public Seggen {
public:
    Roadgen(const Rectangle rect, unsigned int& seedp);
    vector<vector<Point>> CreateRoads(vector<class City>& cities);

private:
    vector<Point> ConnectCities(class City const& city1, class City const& city2);
    set<pair<const City*, const City*>> already_connected = {};

    Roadgen(const Roadgen&) = delete;
    Roadgen& operator=(const Roadgen&) = delete;
};

#endif  // ENGINE_MAPGEN_ROADGEN_H_

// vim: ts=4:sw=4:sts=4:expandtab
