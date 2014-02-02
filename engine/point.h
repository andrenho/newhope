// Copyright 2014 <Imperial Software>

#ifndef ENGINE_POINT_H_
#define ENGINE_POINT_H_

class Point {
public:
	Point(double x, double y)
		: x(x), y(y) {}
	// automatic copy constructor
	Point& operator=(const Point& other) { 
		x = other.X(); 
		y = other.Y(); 
		return *this;
	} 

	double X() const { return x; }
	double Y() const { return y; }

private:

	double x, y;
};

#endif  // ENGINE_POINT_H_
