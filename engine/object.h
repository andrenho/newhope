// Copyright 2014 <Imperial Software>

#ifndef ENGINE_OBJECT_H_
#define ENGINE_OBJECT_H_

#include "engine/point.h"

class Object {
public:
	Object() { }
	virtual ~Object() { }

	virtual Point Position() const = 0;

private:
	Object(const Object&);
	Object& operator=(const Object&);

};

#endif  // ENGINE_OBJECT_H_
