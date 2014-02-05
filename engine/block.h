#ifndef ENGINE_BLOCK_H_
#define ENGINE_BLOCK_H_

#include <cstdint>

class Block final {
public:
	static void StaticInitialization();
	static void StaticDeletion();

	explicit Block(bool crossable)
		: Block(crossable, 0, 0, 0) {}

	Block(bool crossable, uint8_t r, uint8_t g, uint8_t b)
		: Crossable(crossable), R(r), G(g), B(b) {}

	const bool Crossable;
	const uint8_t R, G, B;

	// terrain
	static const Block *EMPTY, *GRASS, *WATER, *DESERT, *SAVANNAH, *BARE,
		           *TROPICAL_FOREST, *TEMPERATE_FOREST, *SHRUBLAND,
			   *TUNDRA, *SNOW;
	
	// objects
	static const Block *FLOOR, *WOODEN_WALL, *DOOR_OPEN;
};

#endif  // ENGINE_BLOCK_H_
