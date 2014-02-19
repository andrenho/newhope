#include <chipmunk.h>

#include <glog/logging.h>
#include <cstdlib>
#include <iostream>

#include "engine/world.h"

#include "./main.h"
#include "engine/city.h"
#include "engine/citylayout.h"
#include "engine/hero.h"
#include "engine/vehicle.h"
#include "engine/mapgen/mapgen.h"

World::World(int x1, int y1, int x2, int y2, unsigned int seedp)
    : Blocks(), x1(x1), y1(y1), x2(x2), y2(y2), seedp(seedp), hero(nullptr), 
      mapgen(MapGen(x1, y1, x2, y2)), objects({}), cities(), physics_ptr({})
{ 
    cpSpaceSetDefaultCollisionHandler(space, CollisionCallback, 
            NULL, NULL, NULL, this);
}


World::~World()
{
    // cleanup physics
    cpBodyEachShape(space->staticBody, FreeStaticShape, this);
}

void
World::Initialize()
{
    // create map
    mapgen.Create();

    // initialize vehicles
    LOG(INFO) << "Initializing vehicles.\n";
    std::shared_ptr<Vehicle> car(new Vehicle(Point(10, 10), VehicleModel::GENERIC));
    AddObject(car);

    // initialize people
    LOG(INFO) << "Initializing hero.\n";
    hero = std::shared_ptr<class Hero>(new class Hero(Point(0, 0) , car));
    AddObject(hero);

    // initialize cities
    LOG(INFO) << "Creating cities.\n";
    CreateCities();

    // create roads
    mapgen.CreateRoads(cities);

    // create people
    LOG(INFO) << "Creating workers.\n";
    AddWorkers();
    LOG(INFO) << "Adding static objects.\n";
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
World::Tiles(Block (&block)[10], int x, int y) const
{
    // TODO - check from cache

    for(auto const& city: cities) {
        if(x >= city.X && x<(city.X+city.W())
        && y >= city.Y && y<(city.Y+city.H())) {
            int n = city.Tiles(block, x, y);
            if(block[0] == Block::EMPTY) {
                block[0] = mapgen.Terrain(x, y);
            }
            return n;
        }
    }

    block[0] = mapgen.Terrain(x, y);
    return 1;
}


bool 
World::IsTileWalkable(int x, int y) const
{
    Block block[10];
    int n = Tiles(block, x, y);

    if(n == 2 && !Blocks.Examine(block[1]).Crossable) {
        return false;
    }
    if(n >= 3 && !Blocks.Examine(block[1]).Crossable) {
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


void 
World::RecalculatePrices()
{
    // TODO
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
    std::unordered_set<Point> positions = mapgen.CitiesPositions(30);
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
            type = City::Type(mapgen.Terrain(x, y));
        }
        cities.emplace_back(x, y, type, 1);
    }

    cities.emplace_back(0, 0, CityType::AGRICULTURAL, 1); // TODO
}


void 
World::AddObject(Object::Ptr obj)
{
    objects.push_back(obj);
    physics_ptr[obj->PhysicsBodyPtr()] = obj;
}


void
World::AddWorkers()
{
    for(auto& city: cities) {
        for(auto const& worker : city.Workers()) {
            std::shared_ptr<Worker> wk = Worker::MakeWorker(worker.second, city, worker.first);
            AddObject(wk);
        }
    }
}


void
World::AddStaticObjects()
{
    for(auto const& city : cities) {
        for(int x=city.X; x<city.X+city.W(); x++) {
            for(int y=city.Y; y<city.Y+city.H(); y++) {
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
    (void) body; (void) data;

    cpSpaceRemoveShape(space, shape);
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
    world->physics_ptr[a]->Collision(world->physics_ptr[b]);
    world->physics_ptr[b]->Collision(world->physics_ptr[a]);

    return 1;
}


// vim: ts=4:sw=4:sts=4:expandtab
