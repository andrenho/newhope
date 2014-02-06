#ifndef ENGINE_RIVERGEN_H_
#define ENGINE_RIVERGEN_H_

#include <map>
#include <vector>
#include "engine/point.h"
#include "engine/rectangle.h"

class Rivergen final {
public:
	Rivergen(const double (&hm)[255][255], const Rectangle rect,
			unsigned int& seedp);
	std::vector<Point> CreateRiver();

private:
	Rivergen(const Rivergen&);
	Rivergen& operator=(const Rivergen&);

	Point RandomPoint() const;
	Point NextPoint(Point& p, std::vector<Point> ignore) const;

	const double (&hm)[255][255];
	const Rectangle rect;
	unsigned int& seedp;
	std::vector<Point> points;
	std::map<Point,double> altitude;
};

#endif  // ENGINE_RIVERGEN_H_
