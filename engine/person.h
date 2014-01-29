// Copyright 2014 <Imperial Software>

#ifndef ENGINE_PERSON_H_
#define ENGINE_PERSON_H_

#include "engine/object.h"

class Person {
public:
	Person() { }
	virtual ~Person() { }

private:
	Person(const Person&);
	Person& operator=(const Person&);

};

#endif  // ENGINE_PERSON_H_
