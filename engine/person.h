// Copyright 2014 <Imperial Software>

#ifndef ENGINE_PERSON_H_
#define ENGINE_PERSON_H_

#include "engine/object.h"

class Person : public Object {
public:
	Person(Point init);
	virtual ~Person() { }

	virtual void InitializePhysics(class cpSpace* space);
	virtual void SetTarget(Point const& p);
	virtual void DestroyPhysics(class cpSpace* space);

	inline virtual double Radius() const { return 0.5; }
	virtual Point Position() const;

protected:
	const Point init;
	class cpBody *body, *target;
	class cpShape* shape;
	class cpConstraint* joint;

private:
	Person(const Person&);
	Person& operator=(const Person&);

};

#endif  // ENGINE_PERSON_H_
