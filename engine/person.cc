#include <chipmunk.h>

#include <glog/logging.h>

#include <algorithm>
#include <cassert>
#include <iostream>

#include "./main.h"
#include "engine/city.h"
#include "engine/person.h"
#include "engine/vehicle.h"
#include "engine/world.h"
#include "engine/npc/banker.h"
#include "engine/npc/shopkeeper.h"
#include "util/stdio.h"

Person::Person(Point init, int money)
    : init(init), body(nullptr), target(nullptr), shape(nullptr), 
      joint(nullptr), vehicle(nullptr), in_vehicle(false), loan(0), wallet(new Wallet(money))
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


Person::~Person()
{
    cpSpaceRemoveShape(space, shape);
    cpSpaceRemoveConstraint(space, joint);
    cpSpaceRemoveBody(space, body);

    cpShapeFree(shape);
    cpConstraintFree(joint);
    cpBodyFree(body);
    cpBodyFree(target);
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
    if(in_vehicle) {
        return vehicle->Position();
    } else {
        cpVect pos = cpBodyGetPos(body);
        return Point(pos.x, pos.y);
    }
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


int 
Person::Money() const 
{ 
    return wallet->Money(); 
}


bool 
Person::Pay(Person& receiver, int value)
{
    if(value > wallet->money) {
        return false;
    } else {
        wallet->money -= value;
        receiver.GetPaid(*this, value);
        return true;
    }
}


void 
Person::GetPaid(Person& giver, int value)
{
    (void) giver;
    wallet->money += value;
}


void 
Person::setLoanValue(Banker& banker, int value)
{
    (void) banker;
    loan = value;
}


bool
Person::PayLoan(class Banker& banker, int value)
{
    (void) banker;

    assert(value <= loan);
    if(Pay(banker, value)) {
       loan -= value;
       return true;
    } else {
        return false;
    }
}

// vim: ts=4:sw=4:sts=4:expandtab
