#include <chipmunk.h>

#include <cstdlib>
#include <iostream>

#include "engine/world.h"

#include "engine/city.h"
#include "engine/citylayout.h"
#include "engine/hero.h"
#include "engine/mapgen.h"
#include "engine/vehicle.h"

World::World(int x1, int y1, int x2, int y2, unsigned int seedp)
    : x1(x1), y1(y1), x2(x2), y2(y2), seedp(seedp), hero(nullptr), 
      space(nullptr), mapgen(new MapGen(x1, y1, x2, y2)), objects({}), 
      cities({}), physics_ptr({})
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
    CreateCities();
    AddWorkers();
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
World::Tiles(const Block* (&block)[10], int x, int y) const
{
    // TODO - check from cache

    for(auto const& city: cities) {
        if(x >= city->X && x<(city->X+city->W())
        && y >= city->Y && y<(city->Y+city->H())) {
            int n = city->Tiles(block, x, y);
            if(block[0] == Block::EMPTY) {
                block[0] = mapgen->Terrain(x, y);
            }
            return n;
        }
    }

    block[0] = mapgen->Terrain(x, y);
    return 1;
}


bool 
World::IsTileWalkable(int x, int y) const
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


void 
World::Limits(int& x1, int& y1, int& x2, int& y2) const
{
    x1 = this->x1;
    y1 = this->y1;
    x2 = this->x2;
    y2 = this->y2;
}


/*****************************************************************************/


double 
World::Random() const
{
    return (static_cast<double>(rand_r(&seedp)) / static_cast<double>(RAND_MAX));
}


void
World::CreateCities()
{
    std::unordered_set<Point> positions = mapgen->CitiesPositions(30);
    for(auto const& pos : positions) {
        int x = static_cast<int>(pos.X()),
            y = static_cast<int>(pos.Y());
        double r = Random();
        CityType type;
        if(r < 0.1) {
            type = CityType::CAPITAL;
        } else if(r < 0.2) {
            type = CityType::FRONTIER;
        } else {
            type = City::Type(mapgen->Terrain(x, y));
        }
        cities.push_back(new City(x, y, type, 1));
    }
}


void 
World::AddObject(Object* obj)
{
    obj->InitializePhysics(space);
    objects.push_back(obj);
    physics_ptr[obj->PhysicsBodyPtr()] = obj;
}


void
World::AddWorkers()
{
    for(auto& city: cities) {
        for(auto& worker : city->Workers()) {
            Worker* wk = new Worker(worker.first, city, worker.second);
            AddObject(wk);
        }
    }
}


void
World::AddStaticObjects()
{
    for(auto const& city : cities) {
        for(int x=city->X; x<city->X+city->W(); x++) {
            for(int y=city->Y; y<city->Y+city->H(); y++) {
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
    (void) body;

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


// vim: ts=4:sw=4:sts=4:expandtab
