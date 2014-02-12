#include "engine/city.h"

#include <cassert>

#include "./globals.h"
#include "engine/world.h"

City::City(int x, int y, CityType type, int n)
    : X(x), Y(y), layout(CityLayouts[CityPair(type, n)]),
      buildings({}), resources({})
{
    for(auto const& bpos : layout.buildings) {
        const Building* b = new Building(
                static_cast<int>(bpos.second.X()), 
                static_cast<int>(bpos.second.Y()), 
                bpos.first.first, bpos.first.second);
        buildings.push_back(b);
    }
    for(Resource res: ResourceList) {
        resources[res] = 250; // TODO
    }
}


City::~City() 
{
    for(auto& building : buildings) {
        delete building;
    }
}


int 
City::Tiles(const Block* (&block)[10], int x, int y) const
{
    for(auto const& b : buildings) {
        if(x >= b->X && x < b->X+b->W()
        && y >= b->Y && y < b->Y+b->H()) {
            return b->Tiles(block, x - b->X, y - b->Y);
        }
    }
    block[0] = Block::EMPTY;
    return 1;
}


CityType 
City::Type(Block const* b)
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
        for(auto const& worker: building->Workers()) {
            pairs.push_back({ worker.first,
                    Point(worker.second.X() + building->X + X,
                          worker.second.Y() + building->Y + Y) });
        }
    }

    return pairs;
}


void 
City::ChangeCargoAmount(Resource res, int amount)
{
    assert(resources.at(res) + amount >= 0);
    resources[res] += amount;
}


// vim: ts=4:sw=4:sts=4:expandtab
