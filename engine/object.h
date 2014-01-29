// Copyright 2014 <Imperial Software>

#ifndef ENGINE_OBJECT_H_
#define ENGINE_OBJECT_H_

#include "engine/point.h"

class Object {
public:
	virtual ~Object() { }

	virtual void InitializePhysics(struct cpSpace* space) { }
	virtual void DestroyPhysics(struct cpSpace* space) { }
	virtual Point Position() const = 0;

protected:
	Object() { }

private:
	Object(const Object&);
	Object& operator=(const Object&);

};

#endif  // ENGINE_OBJECT_H_
