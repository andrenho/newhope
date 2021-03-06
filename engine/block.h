#ifndef ENGINE_BLOCK_H_
#define ENGINE_BLOCK_H_

#include <cstdint>
#include <map>
#include <vector>
using namespace std;


enum Block : uint8_t { 
    // terrain
    EMPTY, GRASS, WATER, DESERT, SAVANNAH, BARE, TROPICAL_FOREST, 
    TEMPERATE_FOREST, SHRUBLAND, TUNDRA, SNOW, BEACH, ASPHALT,

    // objects
    FLOOR, WOODEN_WALL, DOOR_OPEN,
};


struct BlockType final {
    explicit BlockType(bool crossable)
        : BlockType(crossable, 255, 255, 255) {}

    BlockType(bool crossable, uint8_t r, uint8_t g, uint8_t b)
        : Crossable(crossable), R(r), G(g), B(b) {}

    BlockType(BlockType&&) = default;
    BlockType(const BlockType&) = delete;
    BlockType& operator=(const BlockType&) = delete;

    const bool Crossable;
    const uint8_t R, G, B;
};


class BlockManager final {
public:
    BlockManager();

    BlockManager(const BlockManager&) = delete;
    BlockManager& operator=(const BlockManager&) = delete;

    inline BlockType const& Examine(Block b) const { return blocks.at(b); }
    vector<Block> TerrainList() const;

private:
    map<Block, BlockType> blocks;
};

#endif  // ENGINE_BLOCK_H_

// vim: ts=4:sw=4:sts=4:expandtab
