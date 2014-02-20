#include "engine/npc/shopkeeper.h"

#include <glog/logging.h>
#include <cassert>
#include <climits>
#include <string>
using namespace std;

#include "./main.h"
#include "engine/vehicle.h"
#include "engine/world.h"
#include "ui/ui.h"
#include "util/stdio.h"

Shopkeeper::Shopkeeper(Point init, class City& city)
    : Worker(init, city, INT_MAX/2 /* TODO */)
{
    for(Resource res: ResourceList) {
        resources[res] = 250; // TODO
    }
}


void
Shopkeeper::TalkToHero()
{
    ui->Dialog().Shopkeeper(*this);
}


void 
Shopkeeper::ChangeCargoAmount(Resource res, int amount)
{
    assert(resources.at(res) + amount >= 0);
    resources[res] += amount;
}


bool
Shopkeeper::Sell(Person& person, Resource resource, unsigned int amount, string& message)
{
    if(amount == 0) {
        return true;
    }

    int price = static_cast<int>(ResourceBuyPrice(resource) * amount);

    // do the player has a car?
    class Vehicle* person_vehicle = &person.Vehicle();
    if(!person_vehicle) {
        message = _("Unfortunately, you don't have a vehicle to transport the cargo.");
        return false;
    }

    // are enough resources available in the city?
    if(ResourceAmount(resource) < amount) {
        message = mprintf(_("Unfortunately, you don't have this much %s."), resource_name(resource).c_str());
        return false;
    }

    // is there space in the vehcile?
    if(amount > person_vehicle->SpaceLeft(resource)) {
        message = mprintf(_("There's not enough space on the vehicle for all this %s."), resource_name(resource).c_str());
        return false;
    }

    // does the person has enough funds?
    if(!person.Pay(*this, price)) {
        message = _("Unfortunately, you don't have enough money.");
        return false;
    }

    // purchase
    person_vehicle->AddCargo(resource, amount);
    ChangeCargoAmount(resource, -static_cast<int>(amount));

    // message
    message = mprintf(_("That'll be $%d."), price);
    LOG(INFO) << amount << " " << resource_name(resource) << " bought for $" << price;

    // world response
    world->RecalculatePrices();

    return true;
}


bool 
Shopkeeper::Buy(Person& person, Resource resource, unsigned int amount, string& message)
{
    if(amount == 0) {
        return false;
    }

    int price = static_cast<int>(ResourceSellPrice(resource) * amount);

    // do the player has a car?
    class Vehicle* person_vehicle = &person.Vehicle();
    if(!person_vehicle) {
        message = _("Unfortunately, you don't have a vehicle to transport the cargo.");
        return false;
    }

    // does the person has enough resources?
    if(amount > person_vehicle->CargoAmount(resource)) {
        message = mprintf(_("There are only %d of %s in this vehicle."), 
                person_vehicle->CargoAmount(resource), resource_name(resource).c_str());
        return false;
    }

    // sell
    if(!Pay(person, price)) {
        message = _("It seems that I don't have this much money...");
        return false;
    }

    ChangeCargoAmount(resource, static_cast<int>(amount));
    person_vehicle->RemoveCargo(resource, amount);

    // message
    message = mprintf(_("Here's $%d. Spend wisely."), price);
    LOG(INFO) << amount << " " << resource_name(resource) << " sold for $" << price;

    // world response
    world->RecalculatePrices();

    return true;
}



// vim: ts=4:sw=4:sts=4:expandtab
