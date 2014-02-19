#ifndef ENGINE_HERO_H_
#define ENGINE_HERO_H_

#include <memory>
using namespace std;

#include "engine/person.h"
#include "engine/point.h"
#include "engine/resources.h"

class Hero final : public Person {
public:
    explicit Hero(Point init);
    Hero(Point init, shared_ptr<class Vehicle> vehicle);
    virtual ~Hero() { }

    void Collision(shared_ptr<Object> obj) override;

private:
    Hero(const Hero&);
    Hero& operator=(const Hero&);
};

#endif  // ENGINE_HERO_H_


// vim: ts=4:sw=4:sts=4:expandtab
