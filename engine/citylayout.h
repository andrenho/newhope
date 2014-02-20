#ifndef ENGINE_CITYLAYOUT_H_
#define ENGINE_CITYLAYOUT_H_

#include <map>
#include <string>
#include <vector>
#include <utility>
using namespace std;

#include "engine/buildinglayout.h"
#include "engine/point.h"

enum CityType {
    AGRICULTURAL, REFINERY, MINING, FORESTAL, CHEMICAL, CAPITAL, FRONTIER,
    __COUNT__
};

typedef pair<CityType, int> CityPair;
typedef pair<BuildingPair, Point> BuildingPosition;

struct CityLayout final {
    CityLayout(int w, int h, vector<BuildingPosition> buildings)
        : w(w), h(h), buildings(buildings) {}
    CityLayout() {}

    CityLayout(CityLayout&&) = default;
    CityLayout(const CityLayout&) = default;
    CityLayout& operator=(const CityLayout&) = delete;

    const int w = 0, h = 0;
    vector<BuildingPosition> buildings = {};
};

extern map<CityPair, CityLayout> CityLayouts;

#endif  // ENGINE_CITYLAYOUT_H_


// vim: ts=4:sw=4:sts=4:expandtab
