#ifndef ENGINE_OBJECT_H_
#define ENGINE_OBJECT_H_

#include <memory>
using namespace std;

#include "engine/point.h"

class Object {
public:
    typedef shared_ptr<Object> Ptr; // type for shared pointer

    Object(const Object&) = delete;
    Object& operator=(const Object&) = delete;
    virtual ~Object() = default;

    virtual struct cpBody* PhysicsBodyPtr() const = 0;
    virtual Point Position() const = 0;

    virtual void Step() {}
    virtual void Collision(shared_ptr<Object> obj) { (void) obj; }

protected:
    Object() { }
};

#endif  // ENGINE_OBJECT_H_


// vim: ts=4:sw=4:sts=4:expandtab
