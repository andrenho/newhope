#include "engine/citylayout.h"

#include <map>
using namespace std;

map<CityPair, CityLayout> CityLayouts = {
    {
        CityPair(CityType::AGRICULTURAL, 1),
        {
            CityLayout(70, 20,
            {
                BuildingPosition(BuildingPair(BuildingType::GENERAL_STORE, 1), Point(0, 0)),
                BuildingPosition(BuildingPair(BuildingType::BAR, 1), Point(17, 0)),
            }),
        },
    }, {
        CityPair(CityType::REFINERY, 1),
        {
            CityLayout(70, 20,
            {
                BuildingPosition(BuildingPair(BuildingType::GENERAL_STORE, 1), Point(0, 0)),
                BuildingPosition(BuildingPair(BuildingType::BANK, 1), Point(9, 0)),
                BuildingPosition(BuildingPair(BuildingType::BAR, 1), Point(17, 0)),
            }),
        },
    }, {
        CityPair(CityType::MINING, 1),
        {
            CityLayout(70, 20,
            {
                BuildingPosition(BuildingPair(BuildingType::GENERAL_STORE, 1), Point(0, 0)),
                BuildingPosition(BuildingPair(BuildingType::BAR, 1), Point(17, 0)),
            }),
        },
    }, {
        CityPair(CityType::FORESTAL, 1),
        {
            CityLayout(70, 20,
            {
                BuildingPosition(BuildingPair(BuildingType::GENERAL_STORE, 1), Point(0, 0)),
                BuildingPosition(BuildingPair(BuildingType::BAR, 1), Point(17, 0)),
            }),
        },
    }, {
        CityPair(CityType::CHEMICAL, 1),
        {
            CityLayout(70, 20,
            {
                BuildingPosition(BuildingPair(BuildingType::GENERAL_STORE, 1), Point(0, 0)),
                BuildingPosition(BuildingPair(BuildingType::BAR, 1), Point(17, 0)),
            }),
        },
    }, {
        CityPair(CityType::CAPITAL, 1),
        {
            CityLayout(70, 20,
            {
                BuildingPosition(BuildingPair(BuildingType::GENERAL_STORE, 1), Point(0, 0)),
                BuildingPosition(BuildingPair(BuildingType::BAR, 1), Point(17, 0)),
            }),
        },
    }, {
        CityPair(CityType::FRONTIER, 1),
        {
            CityLayout(70, 20,
            {
                BuildingPosition(BuildingPair(BuildingType::GENERAL_STORE, 1), Point(0, 0)),
                BuildingPosition(BuildingPair(BuildingType::BAR, 1), Point(17, 0)),
            }),
        },
    },
};


// vim: ts=4:sw=4:sts=4:expandtab
