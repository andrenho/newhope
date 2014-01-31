// Copyright 2014 <Imperial Software>

#include <chipmunk.h>

#include "engine/world.h"

#include "engine/city.h"
#include "engine/citylayout.h"
#include "engine/hero.h"
#include "engine/mapgen.h"
#include "engine/vehicle.h"

World::World(int x1, int y1, int x2, int y2)
	: x1(x1), y1(y1), x2(x2), y2(y2), hero(nullptr), space(nullptr),
	  mapgen(new MapGen(x1, y1, x2, y2))
{ 
}


World::~World()
{
	delete mapgen;

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
	// create map
	mapgen->Create();

	// initialize physics
	space = cpSpaceNew();
	cpSpaceSetDefaultCollisionHandler(space, CollisionCallback, 
			NULL, NULL, NULL, this);

	// initialize vehicles
	Vehicle* car = new Vehicle(Point(10, 10), VehicleModel::GENERIC);
	AddObject(car);

	// initialize people
	hero = new class Hero(Point(0, 0), car);
	AddObject(hero);

	// initialize cities
	cities.push_back(new City(Point(0, 0), CityType::AGRICULTURAL, 1));
	AddStaticObjects();
}


void
World::Step()
{
	cpSpaceStep(space, 1.0/60.0);
	for(auto& obj : objects) {
		obj->Step();
	}
}


int
World::Tiles(const Block* (&block)[10], int x, int y)
{
	// TODO - check from cache

	for(auto const& city: cities) {
		if(x >= city->Position.X() && x<(city->Position.X()+city->W())
		&& y >= city->Position.Y() && y<(city->Position.Y()+city->H())) {
			int n = city->Tiles(block, x, y);
			if(block[0] == Block::EMPTY) {
				block[0] = Block::GRASS; // TODO
			}
			return n;
		}
	}

	// TODO - read from mapgen

	block[0] = Block::GRASS;
	return 1;
}


bool 
World::IsTileWalkable(int x, int y)
{
	const Block* block[10];
	int n = Tiles(block, x, y);

	if(n == 2 && !block[1]->Crossable) {
		return false;
	}
	if(n >= 3 && !block[2]->Crossable) {
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
	physics_ptr[obj->PhysicsBodyPtr()] = obj;
}


void
World::AddStaticObjects()
{
	for(auto const& city : cities) {
		for(int x=city->Position.X(); x<city->Position.X()+city->W(); x++) {
			for(int y=city->Position.Y(); y<city->Position.Y()+city->H(); y++) {
				if(!IsTileWalkable(x, y)) {
					AddStaticObject(x+0.5, y+0.5, 1, 1);
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


int 
World::CollisionCallback(struct cpArbiter *arb, struct cpSpace *space, 
		void *data)
{
	World* world = static_cast<World*>(data);

	cpBody *a, *b;
	cpArbiterGetBodies(arb, &a, &b);

	// ingnore static objects (TODO)
	if(a == space->staticBody || b == space->staticBody)
		return 1;

	// call both objects 'collision'
	world->physics_ptr[a]->Collision(*world->physics_ptr[b]);
	world->physics_ptr[b]->Collision(*world->physics_ptr[a]);

	return 1;
}
