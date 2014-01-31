// Copyright 2014 <Imperial Software>

#ifndef ENGINE_RECTANGLE_H_
#define ENGINE_RECTANGLE_H_

#include "engine/point.h"

class Rectangle {
public:
	Rectangle(Point const& p1, Point const& p2)
		: p1(p1), p2(p2) {}
	// automatic copy constructor

	Point const& P1() const { return p1; }
	Point const& P2() const { return p2; }

	bool IsInside(Point const& p) const {
		return p.X() >= p1.X() && p.Y() >= p1.Y() 
		    && p.X() <= p2.X() && p.Y() <= p2.Y();
	}

private:
	Rectangle& operator=(const Rectangle&);

	const Point p1, p2;
};

#endif  // ENGINE_RECTANGLE_H_
