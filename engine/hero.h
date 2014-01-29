// Copyright 2014 <Imperial Software>

#ifndef ENGINE_HERO_H_
#define ENGINE_HERO_H_

#include "engine/person.h"
#include "engine/point.h"

class Hero : public Person {
public:
	explicit Hero(Point init) : Person(init) { }
	virtual ~Hero() { }

private:
	Hero(const Hero&);
	Hero& operator=(const Hero&);

};

#endif  // ENGINE_HERO_H_
