#include <cfloat>
#include <cmath>
#include <ctime>

#include "./globals.h"
#include "engine/block.h"
#include "engine/mapgen.h"
#include "engine/point.h"
#include "engine/rectangle.h"
#include "engine/world.h"

MapGen::MapGen(int x1, int y1, int x2, int y2)
	: MapGen(x1, y1, x2, y2, time(nullptr))
{
}


MapGen::MapGen(int x1, int y1, int x2, int y2, unsigned int seed)
	: rect(Rectangle(Point(x1, y1), Point(x2, y2)))
{
	srand(seed);
}


MapGen::~MapGen()
{
}


void
MapGen::Create()
{
	CreatePoints(NUMPOINTS);
	CreateHeightmap();
	ApplyHeightmap();
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
	for(int i=0; i<npoints; i++) {
		int x = Random() * (-rect.P1().X()+rect.P2().X()) + rect.P1().X();
		int y = Random() * (-rect.P1().Y()+rect.P2().Y()) + rect.P1().Y();
		points.push_back(Point(x,y));
		data[Point(x,y)] = PointData();
	}
}


void
MapGen::CreateHeightmap()
{
	// clear up
	for(int x=0; x<255; x++) {
		for(int y=0; y<255; y++) {
			hm[x][y] = 0;
		}
	}

	// create hills
	for(int i=0; i<500; i++) {
		int x, y;
		double r;
		RandomOffcentre(x, y, r);
		CreateHill(x, y, r);
	}

	// normalize
	int max_alt = 0;
	for(int x=0; x<255; x++) {
		for(int y=0; y<255; y++) {
			if(hm[x][y] > max_alt) { 
				max_alt = hm[x][y]; 
			}
		}
	}
	for(int x=0; x<255; x++) {
		for(int y=0; y<255; y++) {
			if(hm[x][y] != 0) {
				hm[x][y] /= max_alt;
			}
		}
	}
}


void
MapGen::ApplyHeightmap()
{
	double prop_w = rect.P1().X() / (rect.P2().X() - rect.P1().X()),
	       prop_h = rect.P1().Y() / (rect.P2().Y() - rect.P1().Y());

	for(auto const& p : points) {
		double prop_x = (p.X() / (rect.P2().X() - rect.P1().X() - prop_w)) * 255,
		       prop_y = (p.Y() / (rect.P2().Y() - rect.P1().Y() - prop_h)) * 255;
		data[p].Altitude = hm[int(prop_x)][int(prop_y)];
		printf("%d\n", data[p].Altitude);
		if(data[p].Altitude <= 0)
			data[p].Biome = Block::OCEAN;
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


double 
MapGen::Random() const
{
	return (static_cast<double>(rand()) / static_cast<double>(RAND_MAX));
}


void 
MapGen::RandomOffcentre(int& x, int& y, double& r) const
{
	r = Random() * 24 + 6;
	double theta = Random() * 2 * M_PI + Random();
	double distance = Random() * (128 - r * 2);
	x = floor(128 + cos(theta) * distance);
	y = floor(128 + cos(theta) * distance);
}


void 
MapGen::CreateHill(int x, int y, double r)
{
	for(int xx=0; xx<255; xx++) {
		for(int yy=0; yy<255; yy++) {
			int alt = pow(r,2) - (pow(xx-x,2) + pow(yy-y,2));
			if(alt > -1) {
				hm[xx][yy] += alt;
			}
		}
	}
}
