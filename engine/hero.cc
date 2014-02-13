#include "engine/hero.h"
#include "engine/vehicle.h"

Hero::Hero(Point init) 
    : Person(init, 2500)
{ 
}


Hero::Hero(Point init, class Vehicle* vehicle)
    : Hero(init)
{
    this->vehicle = vehicle;
}


void 
Hero::Collision(Object& obj)
{
    class Vehicle* vehicle = nullptr;
    class Person* person = nullptr;

    // if it's a car, enter in it
    if((vehicle = dynamic_cast<class Vehicle*>(&obj)) != nullptr) {
        if(vehicle == this->vehicle) {  // is my car?
            in_vehicle = true;
            SetPosition(Point(-100000, -100000));
        }
    // if it's a person, talk to him
    } else if((person = dynamic_cast<class Person*>(&obj)) != nullptr) {
        person->TalkToHero();
    }
}


// vim: ts=4:sw=4:sts=4:expandtab
