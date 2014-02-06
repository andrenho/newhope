#include "engine/block.h"

// terrain
const Block* Block::EMPTY 		= new Block(true );
const Block* Block::GRASS 		= new Block(false,  98, 153, 112);
const Block* Block::WATER 		= new Block(false,  54,  54,  97);
const Block* Block::DESERT 		= new Block(false, 212, 192, 155);
const Block* Block::SAVANNAH		= new Block(false, 153, 180, 112);
const Block* Block::BARE 		= new Block(false, 130, 130, 130);
const Block* Block::TROPICAL_FOREST 	= new Block(false, 112, 166,  98); 
const Block* Block::TEMPERATE_FOREST	= new Block(false,  85, 139, 112);
const Block* Block::SHRUBLAND	 	= new Block(false, 196, 204 ,187);
const Block* Block::TUNDRA	 	= new Block(false, 193, 193, 180);
const Block* Block::SNOW	 	= new Block(false, 248, 248, 248);
const Block* Block::BEACH 		= new Block(false, 182, 162, 125);

//blocks
const Block* Block::FLOOR 	= new Block(false);
const Block* Block::WOODEN_WALL = new Block(false);
const Block* Block::DOOR_OPEN 	= new Block(true );
