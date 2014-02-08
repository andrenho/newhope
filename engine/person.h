#ifndef ENGINE_PERSON_H_
#define ENGINE_PERSON_H_

#include "engine/object.h"

class Person : public Object {
public:
    virtual ~Person() { }

    virtual void InitializePhysics(struct cpSpace* space);
    virtual void SetTarget(Point const& p);
    virtual void SetPosition(Point const& p);
    virtual void DestroyPhysics(struct cpSpace* space);

    virtual void TalkToHero() {}

    inline bool InVehicle() const { return in_vehicle; }
    inline class Vehicle* Vehicle() const { return vehicle; }
    bool ExitVehicle();

    inline virtual double Radius() const { return 0.5; }
    virtual Point Position() const;
    inline struct cpBody* PhysicsBodyPtr() const { return body; }
    inline int Money() const { return money; }

protected:
    explicit Person(Point init);

    const Point init;
    struct cpBody *body, *target;
    struct cpShape* shape;
    struct cpConstraint* joint;
    class Vehicle* vehicle;
    bool in_vehicle;
    int money;

private:
    Person(const Person&);
    Person& operator=(const Person&);
};

#endif  // ENGINE_PERSON_H_


// vim: ts=4:sw=4:sts=4:expandtab
