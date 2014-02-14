#ifndef ENGINE_BUILDING_H_
#define ENGINE_BUILDING_H_

#include <vector>

#include "engine/buildinglayout.h"
#include "engine/block.h"
#include "engine/point.h"

class Building final {
public:
    Building(int x, int y, BuildingType type, int n);
    Building(Building&&) = default;

    int Tiles(Block (&block)[10], int x, int y) const;

    inline int W() const { return layout.w; }
    inline int H() const { return layout.h; }

    inline const std::vector<WorkerPair> Workers() const { return layout.workers; }

    const int X, Y;

private:
    Building(const Building&);
    Building& operator=(const Building&);

    const BuildingType type;
    BuildingLayout const& layout;
};

#endif  // ENGINE_BUILDING_H_


// vim: ts=4:sw=4:sts=4:expandtab
