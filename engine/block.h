#ifndef ENGINE_BLOCK_H_
#define ENGINE_BLOCK_H_

#include <cstdint>
#include <vector>

class Block final {
public:
	static void StaticInitialization();
	static void StaticDeletion();

	explicit Block(bool crossable)
		: Block(crossable, 255, 255, 255) {}

	Block(bool crossable, uint8_t r, uint8_t g, uint8_t b)
		: Crossable(crossable), R(r), G(g), B(b) {}

	static std::vector<Block const*> TerrainList();

	const bool Crossable;
	const uint8_t R, G, B;

	// terrain
	static const Block *EMPTY, *GRASS, *WATER, *DESERT, *SAVANNAH, *BARE,
		           *TROPICAL_FOREST, *TEMPERATE_FOREST, *SHRUBLAND,
			   *TUNDRA, *SNOW, *BEACH;
	
	// objects
	static const Block *FLOOR, *WOODEN_WALL, *DOOR_OPEN;
};

#endif  // ENGINE_BLOCK_H_
