#include <cfloat>
#include <cmath>
#include <ctime>

#include <algorithm>
#include <iostream>

#include "./globals.h"
#include "engine/block.h"
#include "engine/mapgen.h"
#include "engine/point.h"
#include "engine/rectangle.h"
#include "engine/rivergen.h"
#include "engine/world.h"

MapGen::MapGen(int x1, int y1, int x2, int y2)
	: MapGen(x1, y1, x2, y2, static_cast<unsigned int>(time(0)))
{
}


MapGen::MapGen(int x1, int y1, int x2, int y2, unsigned int seed)
	: rect(Rectangle(Point(x1, y1), Point(x2, y2))), seedp(seed),
	  points({}), data({}), rivers({}), tile_cache({}), river_tiles({})
{
}


MapGen::~MapGen()
{
}


void
MapGen::Create()
{
	// create land
	CreatePoints(NUMPOINTS);
	CreateHeightmap();
	
	// create rivers
	Rivergen rivergen(hm, rect, seedp);
	for(int i=0; i<12; i++) {
		rivers.push_back(rivergen.CreateRiver());
	}
	AddRiverTiles();

	// find biomes
	CalculateMoisture();
	CreateBiomes();
	CreateBeaches();
}


Block const* 
MapGen::Terrain(int x, int y) const
{
	Point pt(x, y);
	std::unordered_map<Point,Block const*>::const_iterator it;
	if((it = tile_cache.find(pt)) != tile_cache.end()) {
		return it->second;
	} else if(river_tiles.find(pt) != river_tiles.end()) {
		return Block::WATER;
	} else {
		Point p = ClosestPoint(x, y);
		Block const* b = data.at(p).Biome;
		tile_cache[Point(x,y)] = b; // XXX - remove to deactivate cache
		return b;
	}
}


std::unordered_set<Point> 
MapGen::CitiesPositions(unsigned int n) const
{
	std::unordered_set<Point> positions;

	while(positions.size() < n) {
		for(auto const& biome : Block::TerrainList()) {
			Point p(0, 0);
			if(RandomPointWithBiome(p, biome, positions)) {
				std::cout << "# " << p.X() << " " << p.Y() << std::endl;
				positions.insert(p);
			} else {
				continue;
			}
		}
	}

	return positions;
}


/**************************************************************************/

void 
MapGen::CreatePoints(int npoints)
{
	for(int i=0; i<npoints; i++) {
		double x = world->Random() * (-rect.P1().X()+rect.P2().X()) + rect.P1().X();
		double y = world->Random() * (-rect.P1().Y()+rect.P2().Y()) + rect.P1().Y();
		points.insert(Point(x,y));
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
	double max_alt = 0;
	for(int x=0; x<255; x++) {
		for(int y=0; y<255; y++) {
			hm[x][y] = sqrt(hm[x][y]);
			if(hm[x][y] > max_alt) { 
				max_alt = hm[x][y]; 
			}
		}
	}
	for(int x=0; x<255; x++) {
		for(int y=0; y<255; y++) {
			if(hm[x][y] > 1) {
				hm[x][y] /= max_alt;
			}
		}
	}

	// apply heightmap
	for(auto const& p : points) {
		data[p].Altitude = PointAltitude(p);
	}
}


void 
MapGen::CalculateMoisture()
{
	// calculate
	double max_moisture = 0;
	for(auto const& p : points) {
		data[p].Moisture = DistanceFromWater(p);
		if(data[p].Moisture > max_moisture) {
			max_moisture = data[p].Moisture;
		}
	}

	// normalize
	for(auto const& p : points) {
		data[p].Moisture = max_moisture - data[p].Moisture;
		data[p].Moisture /= max_moisture;
	}
}


void 
MapGen::CreateBiomes()
{
	/* ALT MOIST 1---------------------------0
 	    1        snow       tundra      bare
            |        temperF    shurbland  savannah
            0        tropF      grass      desert */
	for(auto const& p : points) {
		if(data[p].Altitude <= 0) {
			data[p].Biome = Block::WATER;
		} else if(data[p].Altitude < 0.3) {
			if(data[p].Moisture < 0.65) {
				data[p].Biome = Block::DESERT;
			} else if(data[p].Moisture < 0.85) {
				data[p].Biome = Block::GRASS;
			} else {
				data[p].Biome = Block::TROPICAL_FOREST;
			}
		} else if(data[p].Altitude < 0.55) {
			if(data[p].Moisture < 0.8) {
				data[p].Biome = Block::SAVANNAH;
			} else if(data[p].Moisture < 0.9) {
				data[p].Biome = Block::SHRUBLAND;
			} else {
				data[p].Biome = Block::TEMPERATE_FOREST;
			}
		} else {
			if(data[p].Moisture < 0.8) {
				data[p].Biome = Block::BARE;
			} else if(data[p].Moisture < 0.9) {
				data[p].Biome = Block::TUNDRA;
			} else {
				data[p].Biome = Block::SNOW;
			}
		}
	}
}


void
MapGen::CreateBeaches()
{
	const double r = 1100; // radius

	// find all points in a circle
	for(auto& p : points) {
		if(data[p].Biome == Block::WATER) {
			continue;
		}
		for(auto& pt : points) {
			if(data[pt].Biome == Block::WATER 
			&& pow(pt.X() - p.X(), 2) + pow(pt.Y() - p.Y(), 2) < pow(r, 2)) {
				data[p].Biome = Block::BEACH;
			}
		}
	}
}


void
MapGen::AddRiverTiles()
{
	river_tiles.insert(Point(-1, -1));

	for(auto const& river : rivers) {
		for(unsigned int i=0; i<river.size()-1; i++) {
			int x1 = static_cast<int>(river[i].X()),   
			    y1 = static_cast<int>(river[i].Y()),
			    x2 = static_cast<int>(river[i+1].X()), 
			    y2 = static_cast<int>(river[i+1].Y());
			int dx = x2-x1, dy = y2-y1;
			for(int x=x1; x<x2; x+=((x1>x2) ? -1 : 1)) {
				int y = y1 + dy * (x-x1) / dx;
				PlotRiverCircle(x, y, 
					static_cast<int>(world->Random() * 10 + 5));
			}
		}
	}
}


void 
MapGen::PlotRiverCircle(int x0, int y0, int r)
{
	int x = r, y = 0;
	int radius_error = 1-x;

	while(x >= y) {
		for(int nx=-x; nx<=x; nx++) {
			AddRiverTile(nx+x0, y+y0);
			AddRiverTile(nx+x0, -y+y0);
		}
		for(int ny=-y; ny<=y; ny++) {
			AddRiverTile(x+x0, ny+y0);
			AddRiverTile(-x+x0, ny+y0);
		}

		y++;
		if(radius_error < 0) {
			radius_error += 2*y+1;
		} else {
			--x;
			radius_error += 2*(y-x+1);
		}
	}
}


void 
MapGen::AddRiverTile(int x, int y)
{
	river_tiles.insert(Point(x,y));
}


double 
MapGen::PointAltitude(Point const& p) const
{
	double prop_w = rect.P1().X() / (rect.P2().X() - rect.P1().X()),
	       prop_h = rect.P1().Y() / (rect.P2().Y() - rect.P1().Y());
	int prop_x = static_cast<int>((p.X() / (rect.P2().X() - rect.P1().X()) - prop_w) * 255),
	    prop_y = static_cast<int>((p.Y() / (rect.P2().Y() - rect.P1().Y()) - prop_h) * 255);
	return hm[prop_x][prop_y];
}


double 
MapGen::DistanceFromWater(Point const& p) const
{
	double min_distance = DBL_MAX;
	for(auto const& river : rivers) {
		for(auto const& pt : river) {
			min_distance = std::min(pt.MH_Distance(p), min_distance);
		}
	}
	return min_distance;
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


Point
MapGen::RandomPoint() const
{
	return Point(world->Random() * (-rect.P1().X()+rect.P2().X()) + rect.P1().X(),
		     world->Random() * (-rect.P1().Y()+rect.P2().Y()) + rect.P1().Y());
}


bool 
MapGen::RandomPointWithBiome(Point& pt, Block const* biome, 
		std::unordered_set<Point> ignore) const
{
	// create vector only with point in biome, and not ignored
	std::vector<Point> pts;
	for(auto const& p : points) {
		int x = static_cast<int>(p.X()),
		    y = static_cast<int>(p.Y());
		if(ignore.find(p) == ignore.end() && Terrain(x, y) == biome) {
			pts.push_back(p);
		}
	}

	if(pts.empty()) {
		return false;
	} else {
		// choose a random point
		int idx = static_cast<int>(world->Random() * points.size());
		std::cout << "<--" << world->Random() << " " << pts.size() << " .. " << idx << "\n";
		pt = pts[idx];
		return true;
	}
}


void 
MapGen::RandomOffcentre(int& x, int& y, double& r) const
{
	r = world->Random() * 24 + 6;
	double theta = world->Random() * 2 * M_PI + world->Random();
	double distance = world->Random() * (128 - r * 2);
	x = static_cast<int>(128 + cos(theta) * distance);
	y = static_cast<int>(128 + sin(theta) * distance);
}


void 
MapGen::CreateHill(int x, int y, double r)
{
	for(int xx=0; xx<255; xx++) {
		for(int yy=0; yy<255; yy++) {
			int alt = static_cast<int>(pow(r,2) - (pow(xx-x,2) + pow(yy-y,2)));
			if(alt > -1) {
				hm[xx][yy] += alt;
			}
		}
	}
}
