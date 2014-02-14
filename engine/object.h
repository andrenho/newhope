#ifndef ENGINE_OBJECT_H_
#define ENGINE_OBJECT_H_

#include <memory>

#include "engine/point.h"

class Object {
public:
    typedef std::shared_ptr<Object> Ptr;

    virtual ~Object() { }

    virtual void Collision(std::shared_ptr<Object> obj) {
        (void) obj;
    }

    virtual struct cpBody* PhysicsBodyPtr() const = 0;
    virtual Point Position() const = 0;

    virtual void Step() {}

protected:
    Object() { }

private:
    Object(const Object&);
    Object& operator=(const Object&);

};

#endif  // ENGINE_OBJECT_H_


// vim: ts=4:sw=4:sts=4:expandtab
