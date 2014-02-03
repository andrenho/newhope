#include <cfloat>

#include "./globals.h"
#include "engine/block.h"
#include "engine/mapgen.h"
#include "engine/point.h"
#include "engine/rectangle.h"
#include "engine/world.h"

MapGen::MapGen(int x1, int y1, int x2, int y2)
	: MapGen(x1, y1, x2, y2, 0)
{
}


MapGen::MapGen(int x1, int y1, int x2, int y2, unsigned int seed)
	: rect(Rectangle(Point(x1, y1), Point(x2, y2))),
	  generator(std::default_random_engine())
{
}


MapGen::~MapGen()
{
}


void
MapGen::Create()
{
	CreatePoints(NUMPOINTS);
}


Block const* 
MapGen::Terrain(int x, int y) const
{
	std::map<Point,Block const*>::const_iterator it = tile_cache.find(Point(x,y));
	if(it != tile_cache.end()) {
		return it->second;
	} else {
		Point p = ClosestPoint(x, y);
		Block const* b = data.at(p).Biome;
		tile_cache[Point(x,y)] = b; // XXX - remove to deactivate cache
		return b;
	}
}


/**************************************************************************/

void 
MapGen::CreatePoints(int npoints)
{
	std::uniform_int_distribution<int> dx(rect.P1().X(), rect.P2().X()),
		                           dy(rect.P1().Y(), rect.P2().Y());

	for(int i=0; i<npoints; i++) {
		int x = dx(generator), y = dy(generator);
		points.push_back(Point(x,y));
		data[Point(x,y)] = PointData();
		if(i < 100) { data[Point(x,y)].Biome = Block::OCEAN; }
	}
}


Point
MapGen::ClosestPoint(int x, int y) const
{
	int quad = 600;

	Point my = Point(x, y);
	Point const* closest = nullptr;
	double min_dist = DBL_MAX;
	for(;;) {
		for(auto const& p: points) {
			if (p.X() > (x-quad) && p.X() < (x+quad) 
			&&  p.Y() > (y-quad) && p.Y() < (y+quad)) {
				double dist = p.Distance(my);
				if(dist < min_dist) {
					min_dist = dist;
					closest = &p;
				}
			}
		}
		
		if(!closest) {
			quad *= 2;
		} else {
			return *closest;
		}
	}
}
