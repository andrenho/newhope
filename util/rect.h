#ifndef LIBS_RECT_H
#define LIBS_RECT_H

#include <algorithm>
#include "util/point.h"
#include "util/logger.h"

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
	template <typename T> Rect(Point<T> p1, Point<T> p2)
		: x(std::min(p1.x, p2.x)),
		  y(std::min(p1.y, p2.y)),
		  w(std::max(p1.x, p2.x) - x),
		  h(std::max(p1.y, p2.y) - y) { }

	const Rect operator+(const Rect& r) const {
		return Rect(r.x+x, r.y+y, r.w+w, r.h+h);
	}

	Rect& operator+=(const Rect& other) {
		x += other.x;
		y += other.y;
		w += other.w;
		h += other.h;
		return *this;
	}

	Rect& operator=(const Rect& other) {
		if(this != &other) {
			x = other.x;
			y = other.y;
			w = other.w;
			h = other.h;
		}
		return *this;
	}

	template<typename T> const Rect operator+(const Point<T>& p) const {
		return Rect(x+p.x, y+p.y, w, h);
	}

	template<typename T> bool ContainsPoint(Point<T> p) const {
		return p.x >= x && p.x < (x+w) && p.y >= y && p.y < (y+h);
	}

	void Debug() const {
		logger.Debug("%d %d %d %d", x, y, w, h);
	}

	constexpr Rect(const Rect& other) :
		x(other.x), y(other.y), w(other.w), h(other.h) { }

	int x, y, w, h;
};

#endif
