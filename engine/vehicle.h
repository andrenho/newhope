#ifndef ENGINE_VEHICLE_H_
#define ENGINE_VEHICLE_H_

#include <utility>
#include <vector>

#include "engine/object.h"
#include "engine/point.h"
#include "engine/resources.h"

class VehicleModel final {
public:
    VehicleModel(double w, double h, unsigned int cargo_slots) 
        : W(w), H(h), CargoSlots(cargo_slots) {}

    const double W, H;
    const unsigned int CargoSlots;

    static const VehicleModel* GENERIC;
};


struct VehicleSteering final {
    bool accelerate;
    bool reverse;
    double wheel;
};


struct CargoSlot final {
    enum Resource Cargo;
    unsigned int Amount;
};
const CargoSlot EmptySlot = { NOTHING, 0 };


class Vehicle final : public Object {
public:
    Vehicle(Point init_pos, const VehicleModel* model);
    ~Vehicle();

    void InitializePhysics(struct cpSpace* space) override;
    void DestroyPhysics(struct cpSpace* space) override;

    void Step() override;

    Point Position() const override;
    double Angle() const;
    CargoSlot const& Cargo(unsigned int slot) const;
    inline VehicleModel const& Model() const { return model; }

    void PhysicsBodies(struct cpBody*& body, 
            struct cpBody*& rear_wheel_body, 
            struct cpBody*& front_wheel_body) const;
    void PhysicsShapes(struct cpShape*& shape, 
            struct cpShape*& rear_wheel_shape, 
            struct cpShape*& front_wheel_shape) const;
    inline struct cpBody* PhysicsBodyPtr() const { return body; }

    VehicleSteering Steering;

private:
    Vehicle(const Vehicle&);
    Vehicle& operator=(const Vehicle&);

    void UpdateFriction(struct cpBody* body);
    void UpdateDrive(struct cpBody* wheel, double maxForce);
    void UpdateTurn(struct cpBody* wheel);

    struct cpVect LateralVelocity(struct cpBody* body);
    struct cpVect ForwardVelocity(struct cpBody* body);

    VehicleModel const& model;
    const Point init_pos;
    std::vector<CargoSlot> cargo_slots;

    struct cpBody *body, *rear_wheel_body, *front_wheel_body;
    struct cpShape *shape, *rear_wheel_shape, *front_wheel_shape;
    struct cpConstraint *rear_wheel_joint1, *rear_wheel_joint2,
                *front_wheel_joint1, *front_wheel_joint2;
};

#endif  // ENGINE_VEHICLE_H_


// vim: ts=4:sw=4:sts=4:expandtab
