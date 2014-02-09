#include <chipmunk.h>

#include "./globals.h"
#include "engine/vehicle.h"
#include "engine/world.h"

const VehicleModel* VehicleModel::GENERIC = new VehicleModel(3, 7, 4);

Vehicle::Vehicle(Point init_pos, const VehicleModel* model)
    : Steering({false, false, 0}), model(*model), init_pos(init_pos), cargo_slots({}),
    body(nullptr), rear_wheel_body(nullptr), front_wheel_body(nullptr),
    shape(nullptr), rear_wheel_shape(nullptr), front_wheel_shape(nullptr),
    rear_wheel_joint1(nullptr), rear_wheel_joint2(nullptr),
    front_wheel_joint1(nullptr), front_wheel_joint2(nullptr)
{
}


Vehicle::~Vehicle()
{
}


void
Vehicle::InitializePhysics(struct cpSpace* space)
{
    // create vehicle
    cpFloat mass = 150;
    cpFloat moment = cpMomentForBox(mass, model.W, model.H);
    body = cpSpaceAddBody(space, cpBodyNew(mass, moment));
    shape = cpSpaceAddShape(space, cpBoxShapeNew2(body, 
                cpBBNew(-(model.W/2), -(model.H/2), 
                    (model.W/2), (model.H/2))));
    cpBodySetPos(body, cpv(init_pos.X(), init_pos.Y()));
    cpShapeSetGroup(shape, 1);

    // create wheels
    cpFloat ww = 0.5, wh = 1.25;
    cpFloat wheel_mass = 50;
    cpFloat wheel_moment = cpMomentForBox(wheel_mass, ww, wh);

    // create rear wheel
    rear_wheel_body = cpSpaceAddBody(space, 
            cpBodyNew(mass, wheel_moment));
    rear_wheel_shape = cpSpaceAddShape(space, 
            cpBoxShapeNew2(rear_wheel_body,
                cpBBNew(-(ww/2), -(wh/2), (ww/2), (wh/2))));
    cpShapeSetGroup(rear_wheel_shape, 1);
    cpBodySetPos(rear_wheel_body, 
            cpv(init_pos.X(), init_pos.Y()-(model.H/2)+1.5));
    rear_wheel_joint1 = cpSpaceAddConstraint(space, 
            cpPivotJointNew(body, rear_wheel_body, 
                cpBodyGetPos(rear_wheel_body)));
    rear_wheel_joint2 = cpSpaceAddConstraint(space, 
            cpRotaryLimitJointNew(body, rear_wheel_body, 
                0, 0));

    // create front wheel
    front_wheel_body = cpSpaceAddBody(space, 
            cpBodyNew(mass, wheel_moment));
    front_wheel_shape = cpSpaceAddShape(space, 
            cpBoxShapeNew2(front_wheel_body,
                cpBBNew(-(ww/2), -(wh/2), (ww/2), (wh/2))));
    cpShapeSetGroup(front_wheel_shape, 1);
    cpBodySetPos(front_wheel_body, 
            cpv(init_pos.X(), init_pos.Y()+(model.H/2)-1.5));
    front_wheel_joint1 = cpSpaceAddConstraint(space, 
            cpPivotJointNew(body, front_wheel_body, 
                cpBodyGetPos(front_wheel_body)));
    front_wheel_joint2 = cpSpaceAddConstraint(space, 
            cpRotaryLimitJointNew(body, front_wheel_body, 
                -M_PI/6, M_PI/6));
}


void 
Vehicle::DestroyPhysics(struct cpSpace* space)
{
    cpSpaceRemoveShape(space, shape);
    cpSpaceRemoveShape(space, rear_wheel_shape);
    cpSpaceRemoveShape(space, front_wheel_shape);
    cpSpaceRemoveConstraint(space, rear_wheel_joint1);
    cpSpaceRemoveConstraint(space, rear_wheel_joint2);
    cpSpaceRemoveConstraint(space, front_wheel_joint1);
    cpSpaceRemoveConstraint(space, front_wheel_joint2);
    cpSpaceRemoveBody(space, body);
    cpSpaceRemoveBody(space, rear_wheel_body);
    cpSpaceRemoveBody(space, front_wheel_body);

    cpShapeFree(shape);
    cpShapeFree(rear_wheel_shape);
    cpShapeFree(front_wheel_shape);
    cpConstraintFree(rear_wheel_joint1);
    cpConstraintFree(rear_wheel_joint2);
    cpConstraintFree(front_wheel_joint1);
    cpConstraintFree(front_wheel_joint2);
    cpBodyFree(body);
    cpBodyFree(rear_wheel_body);
    cpBodyFree(front_wheel_body);
}


Point 
Vehicle::Position() const
{
    cpVect pos = cpBodyGetPos(body);
    return Point(pos.x, pos.y);
}


double 
Vehicle::Angle() const
{
    return cpBodyGetAngle(body);
}


CargoSlot const& 
Vehicle::Cargo(unsigned int slot) const
{
    if(slot > Model().CargoSlots) {
        abort();
    } else if(slot >= cargo_slots.size()) {
        return EmptySlot;
    } else {
        return cargo_slots[slot];
    }
}


void
Vehicle::Step()
{
    UpdateFriction(front_wheel_body);
    UpdateFriction(rear_wheel_body);
    UpdateDrive(front_wheel_body, 150);
    UpdateDrive(rear_wheel_body, 150);
    UpdateTurn(front_wheel_body);
}


void 
Vehicle::PhysicsBodies(struct cpBody*& body, 
        struct cpBody*& rear_wheel_body, 
        struct cpBody*& front_wheel_body) const
{
    body = this->body;
    rear_wheel_body = this->rear_wheel_body;
    front_wheel_body = this->front_wheel_body;
}


void 
Vehicle::PhysicsShapes(struct cpShape*& shape, 
        struct cpShape*& rear_wheel_shape, 
        struct cpShape*& front_wheel_shape) const
{
    shape = this->shape;
    rear_wheel_shape = this->rear_wheel_shape;
    front_wheel_shape = this->front_wheel_shape;
}


/**************************************************************************/


void 
Vehicle::UpdateFriction(struct cpBody* body)
{
    // kill lateral velocity
    const cpFloat max_lateral_impulse = 400;
    cpVect impulse = cpvmult(cpvneg(LateralVelocity(body)), 
            cpBodyGetMass(body));
    if(cpvlength(impulse) > max_lateral_impulse) {
        impulse = cpvmult(impulse, 
                max_lateral_impulse / cpvlength(impulse));
    }
    cpBodyApplyImpulse(body, impulse, cpvzero);

    // kill angular velocity?
    // cpFloat inertia = cpBodyGetMoment(body);
    cpFloat av = cpBodyGetAngVel(body);
    if(av < 0.0001 || av > 0.0001)
        cpBodySetAngVel(body, av / 1.2);
    
    // apply drag
    cpVect forward_normal = ForwardVelocity(body);
    cpFloat forward_speed = cpvlength(forward_normal);
    if(forward_speed < 1) {
        cpBodySetVel(body, cpvzero);
    } else {
        forward_normal = cpvnormalize(forward_normal);
        cpFloat drag = -1 * forward_speed;
        cpBodyApplyImpulse(body, cpvmult(forward_normal, drag), cpvzero);
    }
}


void 
Vehicle::UpdateDrive(struct cpBody* wheel, double maxForce)
{
    const cpFloat max_forward_speed = 100;
    const cpFloat max_backward_speed = -20;

    cpFloat desired_speed = 0;

    // find desired speed
    if(Steering.accelerate) {
        desired_speed = max_forward_speed;
    } else if(Steering.reverse) {
        desired_speed = max_backward_speed;
    }

    // find speed
    cpVect forward_normal = cpvperp(cpvforangle(cpBodyGetAngle(wheel)));
    cpFloat speed = cpvdot(ForwardVelocity(wheel), forward_normal);

    // apply force
    cpFloat force = 0;
    if(desired_speed > speed) {
        force = maxForce;
    } else if(desired_speed < speed) {
        force = -maxForce;
    } else {
        return;
    }
    cpBodyApplyImpulse(wheel, cpvmult(forward_normal, force), cpvzero);
}


void 
Vehicle::UpdateTurn(struct cpBody* wheel)
{
    cpFloat desired_torque = 0;
    if(Steering.wheel < 0)
        desired_torque = -1100;
    else if(Steering.wheel > 0)
        desired_torque = 1100;
    cpBodySetTorque(wheel, desired_torque);
}


cpVect 
Vehicle::LateralVelocity(struct cpBody* body)
{
    cpVect normal = cpvforangle(cpBodyGetAngle(body));
    return cpvmult(normal, cpvdot(normal, cpBodyGetVel(body)));
}


cpVect 
Vehicle::ForwardVelocity(struct cpBody* body)
{
    cpVect normal = cpvperp(cpvforangle(cpBodyGetAngle(body)));
    return cpvmult(normal, cpvdot(normal, cpBodyGetVel(body)));
}


// vim: ts=4:sw=4:sts=4:expandtab
