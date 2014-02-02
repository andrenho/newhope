// Copyright 2014 <Imperial Software>

#ifndef ENGINE_POINT_H_
#define ENGINE_POINT_H_

#include <cmath>
#include <cstdlib>

class Point {
public:
	Point(double x, double y)
		: x(x), y(y), id(x * 1000000 + y) {} // TODO

	// automatic copy constructor
	Point& operator=(const Point& other) { 
		x = other.X(); 
		y = other.Y(); 
		return *this;
	} 

	bool operator<(const Point& rhs) const {
		return id < rhs.id;
	}

	double MH_Distance(Point const& p) const {
		return abs(x-p.X()) + abs(y-p.Y());
	}

	double Distance(Point const& p) const {
		return sqrt(pow(x-p.X(), 2) + pow(y-p.Y(), 2));
	}

	double X() const { return x; }
	double Y() const { return y; }

private:
	double x, y;
	double const id;
};

#endif  // ENGINE_POINT_H_
