#include "engine/block.h"

// terrain
const Block* Block::EMPTY 	= new Block(true );
const Block* Block::GRASS 	= new Block(false);
const Block* Block::OCEAN 	= new Block(false);

//blocks
const Block* Block::FLOOR 	= new Block(false);
const Block* Block::WOODEN_WALL = new Block(false);
const Block* Block::DOOR_OPEN 	= new Block(true );
