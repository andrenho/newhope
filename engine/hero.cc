// Copyright 2014 <Imperial Software>

#include "engine/hero.h"
#include "engine/vehicle.h"

Hero::Hero(Point init) 
	: Person(init)
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
	class Vehicle* vehicle;
	if((vehicle = dynamic_cast<class Vehicle*>(&obj)) != nullptr) {
		if(vehicle == this->vehicle) {  // is my car?
			in_vehicle = true;
			SetPosition(Point(-100000, -100000));
		}
	}
}


Point 
Hero::Position() const
{
	if(in_vehicle) {
		return vehicle->Position();
	} else {
		return Person::Position();
	}
}
