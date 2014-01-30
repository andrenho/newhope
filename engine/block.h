// Copyright 2014 <Imperial Software>

#ifndef ENGINE_BLOCK_H_
#define ENGINE_BLOCK_H_

#include <cstdint>

class Block {
public:
	Block(bool crossable)
		: Crossable(crossable) {}

	const bool Crossable;

	// terrain
	static Block *EMPTY, *GRASS, *OCEAN,
	
	// objects
	*FLOOR, *WOODEN_WALL, *DOOR_OPEN;
};

#endif  // ENGINE_BLOCK_H_
