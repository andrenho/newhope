// Copyright 2014 <Imperial Software>

#include "engine/world.h"
#include "engine/hero.h"

World::World(int x1, int y1, int x2, int y2)
	: x1(x1), y1(y1), x2(x2), y2(y2), hero(nullptr)
{ 
}


World::~World()
{
	for(auto& obj : objects) {
		delete obj;
	}
}


void
World::Initialize()
{
	hero = new class Hero();
	objects.push_back(hero);
}


int
World::Tiles(Block (&block)[10], int x, int y)
{
	block[0] = Block::GRASS;
	return 1;
}
