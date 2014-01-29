// Copyright 2014 <Imperial Software>

#include <chipmunk.h>

#include "engine/world.h"
#include "engine/hero.h"

World::World(int x1, int y1, int x2, int y2)
	: x1(x1), y1(y1), x2(x2), y2(y2), hero(nullptr), space(nullptr)
{ 
}


World::~World()
{
	for(auto& obj : objects) {
		obj->DestroyPhysics(space);
		delete obj;
	}

	//cpBodyEachShape(space->staticBody, free_static_shape, NULL);
	cpSpaceFree(space);
}


void
World::Initialize()
{
	// initialize physics
	space = cpSpaceNew();
	//cpSpaceSetDefaultCollisionHandler(space, collision_callback, 
	//		NULL, NULL, NULL, L);

	// initialize people
	hero = new class Hero(Point(0, 0));
	AddObject(hero);
}


void
World::Step()
{
	cpSpaceStep(space, 1.0/60.0);
}


int
World::Tiles(Block (&block)[10], int x, int y)
{
	block[0] = Block::GRASS;
	return 1;
}


void 
World::AddObject(Object* obj)
{
	obj->InitializePhysics(space);
	objects.push_back(obj);
}
