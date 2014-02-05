#ifndef ENGINE_RIVERGEN_H_
#define ENGINE_RIVERGEN_H_

#include <vector>
#include "engine/point.h"
#include "engine/rectangle.h"

class Rivergen {
public:
	Rivergen(const double (&hm)[255][255], const Rectangle rect);
	std::vector<Point> CreateRiver();

private:
	Rivergen(const Rivergen&);
	Rivergen& operator=(const Rivergen&);

	const double (&hm)[255][255];
	const Rectangle rect;
};

#endif  // ENGINE_RIVERGEN_H_
