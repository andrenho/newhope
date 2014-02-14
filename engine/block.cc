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

/*
// terrain
    BlockManager::blocks.emplace(EMPTY, Ptr(new BlockType(true ));
    BlockManager::blocks.emplace(GRASS, Ptr(new BlockType(false,  98, 153, 112));
    BlockManager::blocks.emplace(WATER, Ptr(new BlockType(false,  54,  54,  97));
    BlockManager::blocks.emplace(DESERT, Ptr(new BlockType(false, 212, 192, 155));
    BlockManager::blocks.emplace(SAVANNAH, Ptr(new BlockType(false, 153, 180, 112));
    BlockManager::blocks.emplace(BARE, Ptr(new BlockType(false, 130, 130, 130));
    BlockManager::blocks.emplace(TROPICAL_FOREST, Ptr(new BlockType(false, 112, 166,  98)); 
    BlockManager::blocks.emplace(TEMPERATE_FOREST, Ptr(new BlockType(false,  85, 139, 112));
    BlockManager::blocks.emplace(SHRUBLAND, Ptr(new BlockType(false, 196, 204 ,187));
    BlockManager::blocks.emplace(TUNDRA, Ptr(new BlockType(false, 193, 193, 180));
    BlockManager::blocks.emplace(SNOW, Ptr(new BlockType(false, 248, 248, 248));
    BlockManager::blocks.emplace(BEACH, Ptr(new BlockType(false, 182, 162, 125));

//blocks
    BlockManager::blocks.emplace(FLOOR, Ptr(new BlockType(false));
    BlockManager::blocks.emplace(WOODEN_WALL, Ptr(new BlockType(false));
    BlockManager::blocks.emplace(DOOR_OPEN, Ptr(new BlockType(true ));
*/

/*
std::vector<BlockP>
Block::TerrainList()
{
    return std::vector<BlockP>({ GRASS, WATER, DESERT, SAVANNAH, 
         BARE, TROPICAL_FOREST, TEMPERATE_FOREST, SHRUBLAND,
         TUNDRA, SNOW, BEACH });
}
*/

// vim: ts=4:sw=4:sts=4:expandtab
