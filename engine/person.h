// Copyright 2014 <Imperial Software>

#ifndef ENGINE_PERSON_H_
#define ENGINE_PERSON_H_

#include "engine/object.h"

class Person : public Object {
public:
	explicit Person(Point init);
	virtual ~Person() { }

	virtual void InitializePhysics(struct cpSpace* space);
	virtual void SetTarget(Point const& p);
	virtual void DestroyPhysics(struct cpSpace* space);

	inline virtual double Radius() const { return 0.5; }
	virtual Point Position() const;

protected:
	const Point init;
	struct cpBody *body, *target;
	struct cpShape* shape;
	struct cpConstraint* joint;

private:
	Person(const Person&);
	Person& operator=(const Person&);

};

#endif  // ENGINE_PERSON_H_
