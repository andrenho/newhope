#ifndef ENGINE_HERO_H_
#define ENGINE_HERO_H_

#include "engine/person.h"
#include "engine/point.h"

class Hero final : public Person {
public:
	explicit Hero(Point init);
	Hero(Point init, class Vehicle* vehicle);
	virtual ~Hero() { }

	void Collision(Object& obj);

	Point Position() const;

private:
	Hero(const Hero&);
	Hero& operator=(const Hero&);
};

#endif  // ENGINE_HERO_H_
