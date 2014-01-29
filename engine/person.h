// Copyright 2014 <Imperial Software>

#ifndef ENGINE_PERSON_H_
#define ENGINE_PERSON_H_

#include "engine/object.h"

class Person : public Object {
public:
	Person() { }
	virtual ~Person() { }

	inline double Radius() const { return 0.5; }

private:
	Person(const Person&);
	Person& operator=(const Person&);

};

#endif  // ENGINE_PERSON_H_
