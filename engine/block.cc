#include "engine/block.h"

#include <memory>
#include <map>


BlockManager::BlockManager()
    : blocks()
{
    blocks.emplace(EMPTY,            BlockType(true ));
    blocks.emplace(GRASS,            BlockType(false,  98, 153, 112));
    blocks.emplace(WATER,            BlockType(false,  54,  54,  97));
    blocks.emplace(DESERT,           BlockType(false, 212, 192, 155));
    blocks.emplace(SAVANNAH,         BlockType(false, 153, 180, 112));
    blocks.emplace(BARE,             BlockType(false, 130, 130, 130));
    blocks.emplace(TROPICAL_FOREST,  BlockType(false, 112, 166,  98)); 
    blocks.emplace(TEMPERATE_FOREST, BlockType(false,  85, 139, 112));
    blocks.emplace(SHRUBLAND,        BlockType(false, 196, 204 ,187));
    blocks.emplace(TUNDRA,           BlockType(false, 193, 193, 180));
    blocks.emplace(SNOW,             BlockType(false, 248, 248, 248));
    blocks.emplace(BEACH,            BlockType(false, 182, 162, 125));

    blocks.emplace(FLOOR,            BlockType(false));
    blocks.emplace(WOODEN_WALL,      BlockType(false));
    blocks.emplace(DOOR_OPEN,        BlockType(true ));
}


std::vector<Block> 
BlockManager::TerrainList() const
{
    return std::vector<Block>({ GRASS, WATER, DESERT, SAVANNAH, 
         BARE, TROPICAL_FOREST, TEMPERATE_FOREST, SHRUBLAND,
         TUNDRA, SNOW, BEACH });
}

// vim: ts=4:sw=4:sts=4:expandtab
