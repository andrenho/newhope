#include "engine/buildinglayout.h"

#include <map>
using namespace std;

map<BuildingPair, BuildingLayout> BuildingLayouts = {
{ 
    BuildingPair(BuildingType::GENERAL_STORE, 1), 
    {
        BuildingLayout(8, 5, 4,
        {
            {
                "        ",
                " ffffff ",
                " ffffff ",
                " ffffff ",
                "        ",
            },{

                "wwwwwwww",
                "w      w",
                "w      w",
                "w      w",
                "wwwwddww",
            },{
                "wwwwwwww",
                "w      w",
                "w      w",
                "w      w",
                "wwwwddww",
            },{
                "wwwwwwww",
                "wwwwwwww",
                "wwwwwwww",
                "wwwwwwww",
                "wwwwwwww",
            },
        },
        {
            { ' ', Block::EMPTY },
            { 'f', Block::FLOOR },
            { 'w', Block::WOODEN_WALL },
            { 'd', Block::DOOR_OPEN },
        },
        {
            { WorkerJob::SHOPKEEPER, Point(1.5, 1.5) },
        }),
    },
},

{
    BuildingPair(BuildingType::BANK, 1), 
    {
        BuildingLayout(8, 5, 4,
        {
            {
                "        ",
                " ffffff ",
                " ffffff ",
                " ffffff ",
                "        ",
            },{

                "wwwwwwww",
                "w      w",
                "w      w",
                "w      w",
                "wwwwddw ",
            },{
                "wwwwwwww",
                "w      w",
                "w      w",
                "w      w",
                "wwwwddw ",
            },{
                "wwwwwwww",
                "wwwwwwww",
                "wwwwwwww",
                "wwwwwwww",
                "wwwwwww ",
            },
        },
        {
            { ' ', Block::EMPTY },
            { 'f', Block::FLOOR },
            { 'w', Block::WOODEN_WALL },
            { 'd', Block::DOOR_OPEN },
        },
        {
            { WorkerJob::BANKER, Point(1.5, 1.5) },
        }),
    },
},

{
    BuildingPair(BuildingType::BAR, 1), 
    {
        BuildingLayout(8, 5, 4,
        {
            {
                "        ",
                " ffffff ",
                " ffffff ",
                " ffffff ",
                "        ",
            },{

                "wwwwwwww",
                "w      w",
                "w      w",
                "w     ww",
                "wwwwddww",
            },{
                "wwwwwwww",
                "w      w",
                "w      w",
                "w     ww",
                "wwwwddww",
            },{
                "wwwwwwww",
                "wwwwwwww",
                "wwwwwwww",
                "wwwwwwww",
                "wwwwwwww",
            },
        },
        {
            { ' ', Block::EMPTY },
            { 'f', Block::FLOOR },
            { 'w', Block::WOODEN_WALL },
            { 'd', Block::DOOR_OPEN },
        },
        {
            { WorkerJob::BARTENDER, Point(1.5, 1.5) },
            { WorkerJob::SMUGGLER, Point(3.5, 1.5) },
        }),
    },

},
};


// vim: ts=4:sw=4:sts=4:expandtab
