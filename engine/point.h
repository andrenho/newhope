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
		id = other.Id();
		return *this;
	} 

	inline bool operator<(const Point& rhs) const {
		return id < rhs.id;
	}

	inline bool operator==(const Point& rhs) const {
		return id == rhs.id;
	}

	inline double MH_Distance(Point const& p) const {
		return abs(x-p.X()) + abs(y-p.Y());
	}

	inline double Distance(Point const& p) const {
		return sqrt(pow(x-p.X(), 2) + pow(y-p.Y(), 2));
	}

	inline double X() const { return x; }
	inline double Y() const { return y; }
	inline double Id() const { return id; }

private:
	double x, y;
	double id;
};

#endif  // ENGINE_POINT_H_
