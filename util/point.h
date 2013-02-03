#ifndef POINT_H
#define POINT_H

#include <cmath>
#include "util/defines.h"

template <typename T> int sgn(T val) { return (T(0) < val) - (val < T(0)); }

template <typename T>
class Point {
public:
	T x, y;
	//mutable std::map<Point, int> distances;
	
	Point() : Point(0, 0) { }
	//Point(T x, T y) : x(x), y(y), elevation(0) { }

	Point (const Point& other) :
		x(other.x), y(other.y) { }
	
	Point& operator=(const Point& other)
	{
		if(this != &other) {
			x = other.x;
			y = other.y;
		}
		return *this;
	}


	inline T Distance(Point p2) const {
		return sqrt(pow(p2.x-x, 2) + pow(p2.y-y, 2));
	}

	inline Point Displace(Point p2, int displ) const {
		T mx = (x+p2.x)/2,
		    my = (y+p2.y)/2;
		T dx = (p2.y-y) / 4;
		T dy = (x-p2.x) / 4;
		return { mx+dx, my+dy };
	}

	inline bool operator==(const Point& p) const {
		return x == p.x && y == p.y;
	}

	inline bool operator<(const Point& p) const {
		return (y*10000+x) < (p.y*10000+p.x);
	}

	inline Point Sum(int x, int y) const {
		return Point { this->x+x, this->y+y };
	}

	template<typename F> Point<F> operator+(Point<F> p) const {
		return Point { this->x+p.x, this->y+p.y };
	}

	template<typename F> Point<F> operator-(Point<F> p) const {
		return Point { this->x-p.x, this->y-p.y };
	}

	template<typename F> operator Point<F>() const {
		return { static_cast<F>(x), static_cast<F>(y) };
	}

protected:
	Point(T x, T y) : x(x), y(y) { }
};


class Tile : public Point<double> {
public:
	Tile(double x, double y) : Point<double>(x, y) { }

	Tile operator+(Tile p) const {
		return Tile(this->x+p.x, this->y+p.y);
	}

	Tile operator-(Tile p) const {
		return Tile(this->x-p.x, this->y-p.y);
	}
};


class TileElevation: public Tile {
public:
	TileElevation(double x, double y) : Tile(x, y), elevation(0) { }
	int elevation;
};


class ScreenPoint : public Point<int> {
public:
	ScreenPoint() : ScreenPoint(0, 0) { }
	ScreenPoint(int x, int y) : Point<int>(x, y) { }
	ScreenPoint(const Tile& t) : 
		ScreenPoint((int)((t.x * (double)TileSize)),
			    (int)((t.y * (double)TileSize))) { }
};


class RelPoint : public Point<int> {
public:
	RelPoint(int x, int y) : Point<int>(x, y) { }

	RelPoint(const Tile& t) : 
		RelPoint((int)((t.x * (double)TileSize)) - *rx,
			 (int)((t.y * (double)TileSize)) - *ry) { }

	static int *rx, *ry;
};

#endif
