// Copyright 2014 <Imperial Software>

#ifndef ENGINE_BLOCK_H_
#define ENGINE_BLOCK_H_

#include <cstdint>

enum class Block : uint8_t {
	// terrain
	EMPTY, GRASS, OCEAN,

	// objects
	FLOOR, WOODEN_WALL, DOOR_OPEN,
};

#endif  // ENGINE_BLOCK_H_
