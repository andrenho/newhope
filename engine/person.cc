#include <chipmunk.h>

#include <glog/logging.h>

#include <algorithm>
#include <cassert>

#include "./globals.h"
#include "engine/city.h"
#include "engine/person.h"
#include "engine/vehicle.h"
#include "engine/world.h"

Person::Person(Point init)
    : init(init), body(nullptr), target(nullptr), shape(nullptr), 
      joint(nullptr), vehicle(nullptr), in_vehicle(false), money(0)
{ 
}


void 
Person::InitializePhysics(struct cpSpace* space)
{
    // get object fields
    cpFloat x = init.X(), y = init.Y();
    
    // create person body
    body = cpSpaceAddBody(space, cpBodyNew(10, INFINITY));
    cpBodySetPos(body, cpv(x, y));

    // create person shape
    shape = cpSpaceAddShape(space, 
            cpCircleShapeNew(body, 0.5, cpvzero));
    cpShapeSetFriction(shape, 0.1);

    // create target body
    target = cpBodyNew(INFINITY, INFINITY);
    cpBodySetPos(target, cpv(x, y));

    // create joint
    joint = cpSpaceAddConstraint(space, 
            cpPivotJointNew2(target, body, cpvzero, cpvzero));
    joint->maxBias = 15.0f;
    joint->maxForce = 30000.0f;
}


void 
Person::SetTarget(Point const& p)
{
    cpBodySetPos(target, cpv(p.X(), p.Y()));
}


void 
Person::SetPosition(Point const& p)
{
    cpBodySetPos(target, cpv(p.X(), p.Y()));
    cpBodySetPos(body, cpv(p.X(), p.Y()));
}


Point 
Person::Position() const
{
    cpVect pos = cpBodyGetPos(body);
    return Point(pos.x, pos.y);
}


bool
Person::ExitVehicle()
{
    if(in_vehicle) {
        Point vpos = vehicle->Position(),
              ppos = Position();
        double fx = vpos.X() + std::max(vehicle->Model().W/2, vehicle->Model().H/2) + 1;
        while(!world->IsTileWalkable(
                    static_cast<int>(fx), 
                    static_cast<int>(ppos.Y()))) {
            ++fx;
            if(fx > 5) {
                return false;
            }
        }
        in_vehicle = false;
        vehicle->Steering.accelerate = false;
        vehicle->Steering.reverse = false;
        SetPosition(Point(fx, vpos.Y()));
        return true;
    }
    return false;
}


void 
Person::DestroyPhysics(struct cpSpace* space)
{
    cpSpaceRemoveShape(space, shape);
    cpSpaceRemoveConstraint(space, joint);
    cpSpaceRemoveBody(space, body);

    cpShapeFree(shape);
    cpConstraintFree(joint);
    cpBodyFree(body);
    cpBodyFree(target);
}


bool
Person::Buy(City const& city, Resource const& resource, unsigned int amount, std::string& message)
{
    // are enough resources available in the city?
    if(city.ResourceAmount(resource) < amount) {
        // TODO - add message
        return false;
    }

    // does the person has enough funds?
    if(money < static_cast<int>(city.ResourceSellPrice(resource) * amount)) {
        // TODO - add message
        return false;
    }

    // is there space in the vehcile?
    if(amount > vehicle->SpaceLeft(resource)) {
        // TODO - add message
        return false;
    }

    // purchase
    // TODO

    LOG(INFO) << "Buy\n";
}


bool 
Person::Sell(City const& city, unsigned int cargo_slot, unsigned int amount, std::string& message)
{
    // does the person has enough resources?
    // does the seller has enough funds? (TODO)
    LOG(INFO) << "Sell\n";
}


// vim: ts=4:sw=4:sts=4:expandtab
