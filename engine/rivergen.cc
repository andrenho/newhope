#include "engine/rivergen.h"

#include <cfloat>
#include <algorithm>

Rivergen::Rivergen(const double (&hm)[255][255], const Rectangle rect,
	unsigned int& seedp)
	: hm(hm), rect(rect), seedp(seedp), points({}), altitude({})
{
	double prop_w = rect.P1().X() / (rect.P2().X() - rect.P1().X()),
	       prop_h = rect.P1().Y() / (rect.P2().Y() - rect.P1().Y());
	for(int i=0; i<1000; i++) {
		// create point
		double x = Random() * (-rect.P1().X()+rect.P2().X()) + rect.P1().X();
		double y = Random() * (-rect.P1().Y()+rect.P2().Y()) + rect.P1().Y();
		Point p = Point(x, y);
		points.push_back(p);

		// find altitude
		int prop_x = static_cast<int>((p.X() / (rect.P2().X() - rect.P1().X()) - prop_w) * 255),
		    prop_y = static_cast<int>((p.Y() / (rect.P2().Y() - rect.P1().Y()) - prop_h) * 255);
		altitude[p] = hm[prop_x][prop_y];
	}
}


std::vector<Point> 
Rivergen::CreateRiver()
{
	std::vector<Point> river;

try_again:
	Point p = RandomPoint();
	while(altitude[p] < 0) {
		p = RandomPoint();
	}

	river.push_back(p);
	while(altitude[p] > 0)
	{
		p = NextPoint(p, river);
		river.push_back(p);
	}
	if(river.size() < 5) {
		goto try_again;
	}

	return river;
}


double 
Rivergen::Random() const
{
	return (static_cast<double>(rand_r(&seedp)) / static_cast<double>(RAND_MAX));
}


Point 
Rivergen::RandomPoint() const
{
	return points.at(static_cast<int>(Random() * points.size()));
}


Point 
Rivergen::NextPoint(Point& p, std::vector<Point> ignore) const
{
	double r = 1000; // radius
	std::vector<Point> pts_in_circle;

	// find all points in a circle
	while(pts_in_circle.empty()) {
		for(auto& pt : points) {
			if(pow(pt.X() - p.X(), 2) + pow(pt.Y() - p.Y(), 2) < pow(r, 2)) {
				if(std::find(ignore.begin(), ignore.end(), pt) == ignore.end()) {
					pts_in_circle.push_back(pt);
				}
			}
		}
		r *= 2;
	}

	// find lowest point
	double min_altitude = DBL_MAX;
	Point next = p;
	for(auto& pt : pts_in_circle) {
		if(pt != p && altitude.at(pt) < min_altitude) {
			min_altitude = altitude.at(pt);
			next = pt;
		}
	}

	printf("%0.2f %0.2f %f\n", next.X(), next.Y(), r);

	return next;
}
