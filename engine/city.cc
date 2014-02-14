#include "engine/city.h"

#include <cassert>

#include "./main.h"
#include "engine/world.h"

City::City(int x, int y, CityType type, int n)
    : X(x), Y(y), layout(CityLayouts[CityPair(type, n)]), buildings()
{
    for(auto const& bpos : layout.buildings) {
        buildings.emplace_back(
                static_cast<int>(bpos.second.X()), 
                static_cast<int>(bpos.second.Y()), 
                bpos.first.first, bpos.first.second);
    }
}


int 
City::Tiles(Block (&block)[10], int x, int y) const
{
    for(auto const& b : buildings) {
        if(x >= b.X && x < b.X+b.W()
        && y >= b.Y && y < b.Y+b.H()) {
            return b.Tiles(block, x - b.X, y - b.Y);
        }
    }
    block[0] = Block::EMPTY;
    return 1;
}


CityType 
City::Type(Block b)
{
    if(b == Block::GRASS) {
        return CityType::AGRICULTURAL;
    } else if(b == Block::SNOW) {
        return CityType::REFINERY;
    } else if(b == Block::BARE) {
        return CityType::MINING;
    } else if(b == Block::TEMPERATE_FOREST || b == Block::TROPICAL_FOREST) {
        return CityType::FORESTAL;
    } else if(b == Block::DESERT || b == Block::BEACH) {
        return CityType::CHEMICAL;
    } else {
        return static_cast<CityType>(world->Random() * CityType::__COUNT__);
    }
}


const std::vector<WorkerPair> 
City::Workers() const
{
    std::vector<WorkerPair> pairs;

    for(auto const& building: buildings) {
        for(auto const& worker: building.Workers()) {
            pairs.push_back({ worker.first,
                    Point(worker.second.X() + building.X + X,
                          worker.second.Y() + building.Y + Y) });
        }
    }

    return pairs;
}


// vim: ts=4:sw=4:sts=4:expandtab
