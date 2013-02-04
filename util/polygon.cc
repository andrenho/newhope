#include "util/polygon.h"

#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <vector>
using namespace std;

#include "util/logger.h"

Polygon::Polygon(Point<int>* points, int n_points)
	: Polygon()
{
	for(int i(0); i<n_points; i++) {
		this->points_.push_back(points[i]);
	}
}


Polygon::Polygon(Rect r)
	: Polygon()
{
	points_.push_back(Point<int>(r.x, r.y));
	points_.push_back(Point<int>((r.x+r.w), r.y));
	points_.push_back(Point<int>((r.x+r.w), (r.y+r.h)));
	points_.push_back(Point<int>(r.x, (r.y+r.h)));
}


void Polygon::FakeVoronoi(unsigned int seed, int w, int h, int density, 
		vector<Polygon*>& polygons)
{
	if(density > 100) {
		logger.Error(1, "Max density 100.");
	}

	int x, y;
	int xx, yy;

	// initialize seed
	srand(seed);

	// add points
	int n_points(0), max_x(0), max_y(0);
	Point<int> points[100][100];
	const int d(w / density / 4);
	for(x=(w/density*3/2), xx=0; x<w; x+=(w/density*3/2)) {
		for(y=(h/density), yy=0; y<h; y+=(h/density)) {
			int mx;
			if(yy % 2 == 0) {
				mx = (xx % 2) ? x-d : x+d;
			} else {
				mx = (xx % 2) ? x+d : x-d;
			}
			points[xx][yy++] = { mx, y };
			n_points++;
		}
		max_y = yy;
		if(x > 0) {
			xx++;
		}
	}
	max_x = xx;

	// disturb points
	int max_disturb(w/density);
	for(x=0; x<max_x; x++) {
		for(y=0; y<max_y; y++) {
			points[x][y].x += (rand() % max_disturb) - max_disturb/2;
			points[x][y].y += (rand() % max_disturb) - max_disturb/2;
		}
	}

	// connect points
	for(y=0; y<(max_y-2); y++) {
		for(x=(y % 2) ? 1 : 0; x<(max_x-1); x+=2) {
			polygons.push_back(new Polygon());
			polygons.back()->points_.push_back(points[x][y]);
			polygons.back()->points_.push_back(points[x+1][y]);
			polygons.back()->points_.push_back(points[x+1][y+1]);
			polygons.back()->points_.push_back(points[x+1][y+2]);
			polygons.back()->points_.push_back(points[x][y+2]);
			polygons.back()->points_.push_back(points[x][y+1]);
			polygons.back()->CalculateLimits();
		}
	}
}


const Point<int> 
Polygon::Midpoint() const
{
	static Point<int> invalid( -1, -1);
	if(midpoint == invalid)
	{
		if(limit_x1 == INT_MAX) {
			CalculateLimits();
		}
		midpoint = (const Point<int>) {
			limit_x1 + (limit_x2 - limit_x1) / 2,
			limit_y1 + (limit_y2 - limit_y1) / 2
		};
	}
	return midpoint;
}


void 
Polygon::CalculateLimits() const
{
	for(const auto& point : points_) {
		int x(point.x);
		if(x < limit_x1)
			limit_x1 = x;
		if(x > limit_x2)
			limit_x2 = x;

		int y(point.y);
		if(y < limit_y1)
			limit_y1 = y;
		if(y > limit_y2)
			limit_y2 = y;
	}
}


void 
Polygon::MidlineDisplacement(int n)
{
	if(n == 0) {
		CalculateLimits();
	} else {
		vector<Point<int>> new_points;
		for(auto point = points_.begin(); point != points_.end(); point++) {
			Point<int> p1(*point);
			Point<int> p2(points_.front());
			if(point+1 != points_.end())
				p2 = *(point+1);
			Point<int> p3(p1.Displace(p2, 6));
			new_points.push_back(p1);
			new_points.push_back(p3);
		}
		points_.swap(new_points);
		
		MidlineDisplacement(n-1);
	}
}


void
Polygon::Debug() const
{
	for(const auto& point : points_) {
		logger.Debug("%d %d", point.x, point.y);
	}
}


bool 
Polygon::ContainsPoint(Point<int> p) const
{
	return find(points_.begin(), points_.end(), p) != points_.end();
}


void 
Polygon::NeighbourPoints(Point<int> p, vector<Point<int>>& neigh_points) const
{
	assert(points_.size() >= 3);

	auto r(find(points_.begin(), points_.end(), p));
	if(r == points_.end()) {
		return;
	}

	if(*r == points_.back()) {
		neigh_points.push_back(points_.front());
	} else {
		neigh_points.push_back(*(r+1));
	}

	if(*r == points_.front()) {
		neigh_points.push_back(points_.back());
	} else {
		neigh_points.push_back(*(r-1));
	}
}


bool 
Polygon::IsTouching(Polygon const& poly)
{
	for(auto const& p: poly.points_) {
		if(find(points_.begin(), points_.end(), p) != points_.end()) {
			return true;
		}
	}
	return false;
}


bool
Polygon::BorderIntersects(Rect const& r)
{
	Point<int> prect[4][2] {
		{ { r.x,     r.y     }, { r.x+r.w, r.y } },
		{ { r.x+r.w, r.y     }, { r.x+r.w, r.y+r.h } },
		{ { r.x+r.w, r.y+r.h }, { r.x,     r.y+r.h } },
		{ { r.x,     r.y+r.h }, { r.x,     r.y } }
	};

	for(unsigned int i(0); i<points_.size()-1; i++)
	{
		Point<int> a(points_[i]),
		           b(points_[i+1]);

		for(int j(0); j<4; j++) {
			Point<int> c(prect[j][0]),
			           d(prect[j][1]);
			bool abc((b.x - a.x) * (c.y - a.y) > 
				 (b.y - a.y) * (c.x - a.x));
			bool abd((b.x - a.x) * (d.y - a.y) > 
				 (b.y - a.y) * (d.x - a.x));
			bool bcd((c.x - b.x) * (d.y - b.y) > 
				 (c.y - b.y) * (d.x - b.x));
			bool acd((c.x - a.x) * (d.y - a.y) > 
				 (c.y - a.y) * (d.x - a.x));
			if((acd != bcd) && (abc != abd)) {
				return true;
			}
		}
	}
	return false;
}
