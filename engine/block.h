#ifndef ENGINE_BLOCK_H_
#define ENGINE_BLOCK_H_

#include <cstdint>

class Block {
public:
	static void StaticInitialization();
	static void StaticDeletion();

	explicit Block(bool crossable)
		: Crossable(crossable) {}

	const bool Crossable;

	// terrain
	static const Block *EMPTY, *GRASS, *OCEAN;
	
	// objects
	static const Block *FLOOR, *WOODEN_WALL, *DOOR_OPEN;
};

#endif  // ENGINE_BLOCK_H_
