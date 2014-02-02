#include "engine/block.h"

// terrain
const Block* Block::EMPTY 	= new Block(true );
const Block* Block::GRASS 	= new Block(false,  98, 153, 112);
const Block* Block::OCEAN 	= new Block(false,  54,  54,  97);

//blocks
const Block* Block::FLOOR 	= new Block(false);
const Block* Block::WOODEN_WALL = new Block(false);
const Block* Block::DOOR_OPEN 	= new Block(true );
