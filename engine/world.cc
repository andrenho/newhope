// Copyright 2014 <Imperial Software>

#include <chipmunk.h>

#include "engine/world.h"

#include "engine/city.h"
#include "engine/citylayout.h"
#include "engine/hero.h"

World::World(int x1, int y1, int x2, int y2)
	: x1(x1), y1(y1), x2(x2), y2(y2), hero(nullptr), space(nullptr)
{ 
}


World::~World()
{
	// delete objects
	for(auto& city : cities) {
		delete city;
	}

	for(auto& obj : objects) {
		obj->DestroyPhysics(space);
		delete obj;
	}

	// cleanup physics
	cpBodyEachShape(space->staticBody, FreeStaticShape, this);
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

	// initialize cities
	cities.push_back(new City(Point(0, 0), CityType::AGRICULTURAL, 1));
	AddStaticObjects();
}


void
World::Step()
{
	cpSpaceStep(space, 1.0/60.0);
}


int
World::Tiles(Block* (&block)[10], int x, int y)
{
	// TODO - check from cache

	for(auto const& city: cities) {
		if(x >= city->Position.X() && x<(city->Position.X()+city->W())
		&& y >= city->Position.Y() && y<(city->Position.Y()+city->H())) {
			return city->Tiles(block, x, y);
		}
	}

	// TODO - read from mapgen

	block[0] = Block::GRASS;
	return 1;
}


bool 
World::IsTileWalkable(int x, int y)
{
	Block* block[10];
	int n = Tiles(block, x, y);

	if(n == 2 && !block[1]->Crossable) {
		return false;
	}
	if(n == 3 && !block[2]->Crossable) {
		return false;
	}

	return true;
}


/*****************************************************************************/


void 
World::AddObject(Object* obj)
{
	obj->InitializePhysics(space);
	objects.push_back(obj);
}


void
World::AddStaticObjects()
{
	for(auto const& city : cities) {
		for(int x=city->Position.X(); x<city->Position.X()+city->W(); x++) {
			for(int y=city->Position.Y(); y<city->Position.Y()+city->H(); y++) {
				if(!IsTileWalkable(x, y)) {
					AddStaticObject(x, y, 1, 1);
				}
			}
		}
	}
}


void 
World::AddStaticObject(double x, double y, double w, double h)
{
	cpShape *shape = cpBoxShapeNew2(space->staticBody, 
			cpBBNew(x-(w/2), y-(h/2), x+(w/2), y+(h/2)));
	cpShapeSetFriction(shape, 1);
	cpSpaceAddShape(space, shape);
}


void 
World::FreeStaticShape(cpBody *body, cpShape *shape, void* data)
{
	World *object = static_cast<World*>(data);
	cpSpaceRemoveShape(object->space, shape);
	cpShapeFree(shape);
}
