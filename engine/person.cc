#include <chipmunk.h>

#include "engine/person.h"
#include "engine/vehicle.h"

Person::Person(Point init)
	: init(init), body(nullptr), target(nullptr), shape(nullptr), 
	  joint(nullptr), vehicle(nullptr), in_vehicle(false)
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
