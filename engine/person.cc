#include <chipmunk.h>

#include <glog/logging.h>

#include <algorithm>
#include <cassert>
#include <iostream>

#include "./globals.h"
#include "engine/city.h"
#include "engine/person.h"
#include "engine/vehicle.h"
#include "engine/world.h"
#include "util/stdio.h"

Person::Person(Point init)
    : init(init), body(nullptr), target(nullptr), shape(nullptr), 
      joint(nullptr), vehicle(nullptr), in_vehicle(false), money(0), loan(0)
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
Person::Buy(City& city, Resource const& resource, unsigned int amount, std::string& message)
{
    if(amount == 0) {
        return true;
    }

    int price = static_cast<int>(city.ResourceBuyPrice(resource) * amount);

    // do the player has a car?
    if(!vehicle) {
        message = _("Unfortunately, you don't have a vehicle to transport the cargo.");
        return false;
    }

    // are enough resources available in the city?
    if(city.ResourceAmount(resource) < amount) {
        message = mprintf(_("Unfortunately, you don't have this much %s."), resource_name(resource).c_str());
        return false;
    }

    // does the person has enough funds?
    if(money < price) {
        message = _("Unfortunately, you don't have enough money.");
        return false;
    }

    // is there space in the vehcile?
    if(amount > vehicle->SpaceLeft(resource)) {
        message = mprintf(_("There's not enough space on the vehicle for all this %s."), resource_name(resource).c_str());
        return false;
    }

    // purchase
    vehicle->AddCargo(resource, amount);
    city.ChangeCargoAmount(resource, -static_cast<int>(amount));
    money -= price;

    // message
    message = mprintf(_("That'll be $%d."), price);
    LOG(INFO) << amount << " " << resource_name(resource) << " bought for $" << price;

    // world response
    world->RecalculatePrices();

    return true;
}


bool 
Person::Sell(City& city, unsigned int cargo_slot, unsigned int amount, std::string& message)
{
    if(amount == 0) {
        return false;
    }

    // do the player has a car?
    if(!vehicle) {
        message = _("Unfortunately, you don't have a vehicle to transport the cargo.");
        return false;
    }

    auto const& slot = vehicle->Cargo(cargo_slot);
    int price = static_cast<int>(city.ResourceSellPrice(slot.Cargo) * amount);

    // does the person has enough resources?
    if(amount > slot.Amount) {
        message = mprintf(_("There are only %d of %s in this cargo slot."), slot.Amount, resource_name(slot.Cargo).c_str());
        return false;
    }

    // does the seller has enough funds? (TODO)
    
    // sell
    Resource res = slot.Cargo;
    vehicle->RemoveCargo(res, amount);
    city.ChangeCargoAmount(res, static_cast<int>(amount));
    money += price;

    // message
    message = mprintf(_("Here's $%d. Spend wisely."), price);
    LOG(INFO) << amount << " " << resource_name(res) << " sold for $" << price;

    // world response
    world->RecalculatePrices();

    return true;
}


int 
Person::Collateral() const
{
    return 50000;
}


void 
Person::setLoanValue(int value)
{
    assert(value <= MaxLoanPossible());
    loan = value;
    money += value;
}


void 
Person::PayLoan(int value)
{
    assert(value <= money);
    assert(value <= loan);
    loan -= value;
    money -= value;
}

// vim: ts=4:sw=4:sts=4:expandtab
