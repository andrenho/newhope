#ifndef ENGINE_BUILDINGLAYOUT_H_
#define ENGINE_BUILDINGLAYOUT_H_

#include <map>
#include <string>
#include <vector>
#include <utility>
using namespace std;

#include "engine/block.h"
#include "engine/worker.h"

enum BuildingType {
    GENERAL_STORE, BANK, BAR,
};

typedef pair<BuildingType, int> BuildingPair;
typedef pair<WorkerJob, Point> WorkerPair;

struct BuildingLayout final {
    BuildingLayout()
        : w(0), h(0), floors(0), floor({}), label({}), workers({}) {}

    BuildingLayout(int w, int h, int floors,
            vector<vector<string>> floor,
            map<const char, const Block> label,
            vector<WorkerPair> workers)
        : w(w), h(h), floors(floors), floor(floor), label(label),
              workers(workers){}

    int w, h, floors;
    vector<vector<string>> floor;
    map<const char, const Block> label;
    vector<WorkerPair> workers;
};

extern map<BuildingPair, BuildingLayout> BuildingLayouts;

#endif  // ENGINE_BUILDINGLAYOUT_H_

// vim: ts=4:sw=4:sts=4:expandtab
