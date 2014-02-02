#ifndef ENGINE_OBJECT_H_
#define ENGINE_OBJECT_H_

#include "engine/point.h"

class Object {
public:
	virtual ~Object() { }

	virtual void InitializePhysics(struct cpSpace* space) { }
	virtual void DestroyPhysics(struct cpSpace* space) { }

	virtual void Collision(Object& obj) { }

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
