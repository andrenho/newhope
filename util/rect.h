#ifndef LIBS_RECT_H
#define LIBS_RECT_H

#include <algorithm>
#include "util/point.h"

class Rect {
public:
	constexpr Rect(int x, int y, int w, int h)
		: x(x), y(y), w(w), h(h) { }

	Rect()
		: x(0), y(0), w(0), h(0) { }
	Rect(int n)
		: x(n), y(0), w(0), h(0) { }
	Rect(int x, int y)
		: x(x), y(y), w(0), h(0) { }
	Rect(Point<int> p1, Point<int> p2)
		: x(std::min(p1.x, p2.x)),
		  y(std::min(p1.y, p2.y)),
		  w(std::max(p1.x, p2.x) - x),
		  h(std::max(p1.y, p2.y) - y) { }

	inline void Add(int x, int y, int w, int h) {
		this->x += x;
		this->y += y;
		this->w += w;
		this->h += h;
	}

	template<typename T> bool ContainsPoint(Point<T> p) const {
		return p.x >= x && p.x < (x+w) && p.y >= y && p.y < (y+h);
	}

	constexpr Rect(const Rect& other) :
		x(other.x), y(other.y), w(other.w), h(other.h) { }

	int x, y, w, h;

private:
	Rect& operator=(const Rect& other); // disallow assignment
};

#endif
