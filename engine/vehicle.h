#ifndef ENGINE_VEHICLE_H_
#define ENGINE_VEHICLE_H_

#include <utility>
#include <vector>
using namespace std;

#include "engine/object.h"
#include "engine/point.h"
#include "engine/resources.h"

typedef shared_ptr<const class VehicleModel> VehicleModelP;

class VehicleModel final {
public:
    VehicleModel(double w, double h, unsigned int cargo_slots) 
        : W(w), H(h), CargoSlots(cargo_slots) {}

    const double W, H;
    const unsigned int CargoSlots;

    static const VehicleModelP GENERIC;
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
    Vehicle(Point init_pos, const VehicleModelP& model);
    ~Vehicle();

    Vehicle(const Vehicle&) = delete;
    Vehicle& operator=(const Vehicle&) = delete;

    void Step() override;

    Point Position() const override;
    double Angle() const;
    CargoSlot const& Cargo(unsigned int slot) const;
    inline VehicleModel const& Model() const { return *model; }

    void AddCargo(Resource res, unsigned int amount);
    void RemoveCargo(Resource res, unsigned int amount);
    unsigned int SpaceLeft(Resource res) const;
    unsigned int CargoAmount(Resource res) const;

    void PhysicsBodies(struct cpBody*& body, struct cpBody*& rear_wheel_body, struct cpBody*& front_wheel_body) const;
    void PhysicsShapes(struct cpShape*& shape, struct cpShape*& rear_wheel_shape, struct cpShape*& front_wheel_shape) const;
    inline struct cpBody* PhysicsBodyPtr() const { return body; }

    VehicleSteering Steering = { false, false, 0 };

private:
    void UpdateFriction(struct cpBody* body);
    void UpdateDrive(struct cpBody* wheel, double maxForce);
    void UpdateTurn(struct cpBody* wheel);

    struct cpVect LateralVelocity(struct cpBody* body);
    struct cpVect ForwardVelocity(struct cpBody* body);

    VehicleModelP const& model;
    const Point init_pos;
    vector<CargoSlot> cargo_slots = {};

    struct cpBody *body = nullptr, *rear_wheel_body = nullptr, *front_wheel_body = nullptr;
    struct cpShape *shape = nullptr, *rear_wheel_shape = nullptr, *front_wheel_shape = nullptr;
    struct cpConstraint *rear_wheel_joint1 = nullptr, *rear_wheel_joint2 = nullptr,
                *front_wheel_joint1 = nullptr, *front_wheel_joint2 = nullptr;
};

#endif  // ENGINE_VEHICLE_H_


// vim: ts=4:sw=4:sts=4:expandtab
