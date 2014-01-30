// Copyright 2014 <Imperial Software>

#include "engine/block.h"

// terrain
Block* Block::EMPTY 		= new Block(true );
Block* Block::GRASS 		= new Block(false);
Block* Block::OCEAN 		= new Block(false);
Block* Block::FLOOR 		= new Block(false);
Block* Block::WOODEN_WALL 	= new Block(false);
Block* Block::DOOR_OPEN 	= new Block(true );
