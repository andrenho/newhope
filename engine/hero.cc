#include "engine/hero.h"

#include <iostream>

using namespace std;

#include "engine/vehicle.h"

Hero::Hero(Point init) 
    : Person(init, 2500)
{ 
}


Hero::Hero(Point init, shared_ptr<class Vehicle> vehicle)
    : Hero(init)
{
    this->vehicle = vehicle;
}


void 
Hero::Collision(shared_ptr<Object> obj)
{
    shared_ptr<class Vehicle> vehicle = dynamic_pointer_cast<class Vehicle>(obj);
    shared_ptr<class Person> person = dynamic_pointer_cast<class Person>(obj);

    // if it's a car, enter in it
    if(vehicle) {
        if(vehicle == this->vehicle) {  // is my car?
            in_vehicle = true;
            SetPosition(Point(-100000, -100000));
        }
    // if it's a person, talk to him
    } else if(person) {
        person->TalkToHero();
    }
}


// vim: ts=4:sw=4:sts=4:expandtab
