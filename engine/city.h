#ifndef ENGINE_CITY_H_
#define ENGINE_CITY_H_

#include <map>
#include <vector>

#include "engine/building.h"
#include "engine/citylayout.h"
#include "engine/point.h"
#include "engine/resources.h"

class City final {
public:
    City(int x, int y, CityType type, int n);
    ~City();

    int Tiles(const Block* (&block)[10], int x, int y) const;

    inline int W() const { return layout.w; }
    inline int H() const { return layout.h; }

    static CityType Type(Block const* biome);
    const std::vector<WorkerPair> Workers() const;

    inline unsigned int ResourceAmount(Resource r) const { return resources.at(r); }
    inline unsigned int ResourceSellPrice(Resource r) const { (void)r; return 5; }
    inline unsigned int ResourceBuyPrice(Resource r) const { (void)r; return 10; }

    void ChangeCargoAmount(Resource res, int amount);

    const int X, Y;

private:
    City(const City&);
    City& operator=(const City&);

    CityLayout const& layout;
    std::vector<const Building*> buildings;
    std::map<Resource, int> resources;
};

#endif  // ENGINE_CITY_H_


// vim: ts=4:sw=4:sts=4:expandtab
