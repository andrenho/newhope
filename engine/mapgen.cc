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
	: MapGen(x1, y1, x2, y2, static_cast<unsigned int>(time(0)))
{
}


MapGen::MapGen(int x1, int y1, int x2, int y2, unsigned int seed)
	: rect(Rectangle(Point(x1, y1), Point(x2, y2))), seedp(seed)
{
}


MapGen::~MapGen()
{
}


void
MapGen::Create()
{
	CreatePoints(NUMPOINTS);
	CreateHeightmap();
	CreateRivers(1);
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

		// TODO
		if(data[p].Altitude <= 0)
			data[p].Biome = Block::OCEAN;
	}
}


void
MapGen::CreateRivers(int nrivers)
{
	for(int i=0; i<nrivers; i++) {

		// get random point
		Point p = RandomPoint();

		// create a new river
		rivers.push_back(std::vector<Point>());
		for(;;) {
			// add point to river
			rivers[i].push_back(p);

			// find if this point is under water, then finish
			if(PointAltitude(p) < 0) {
				break;
			}

			// find next point
			Point next = p;
			double min_altitude = 1;
			for(int j=0; j<12; j++) {
				double angle = Random() * 2 * M_PI;
				double length = Random() * 100 + 100;
				Point np(p.X() + cos(angle) * length,
				         p.Y() + sin(angle) * length);
				if(!rect.ContainsPoint(np)) {
					continue;
				}
				double altitude = PointAltitude(np);
				if(altitude < min_altitude) {
					min_altitude = altitude;
					next = np;
				}
			}
			if(next == p) { // no next point found
				break;
			}
			p = next;
		}
		printf("%d\n", rivers[i].size());
	}

/*
   for i=1,12 do
      ::try_again::

      -- choose a random point in land
      local p = self.plane:random_point()
      local points_used = { p }
      local river_pts = { p }

      while p.altitude > 0 do
         
         -- if the river is getting to sea, end it
         for _,poly in ipairs(self.plane.polygons) do
            for _,pt in ipairs(poly.points) do
               if table.find(river_pts, pt) then
                  if poly.altitude <= 0 then
                     goto done
                  end
               end
            end
         end

         -- find next point that contains the lowest altitude, ignoring the points already used
         local np, lowest_alt = nil, math.huge
         for _,seg in ipairs(self.plane:segments_containing_endpoint(p)) do
            -- find next point
            local op = seg.startpoint
            if op == p then op = seg.endpoint end
            -- find lowest point
            if not table.find(points_used, op) then
               if op.altitude < lowest_alt then
                  lowest_alt = op.altitude
                  np = op
               end
            end
         end
         if not np then break end -- no point was found (TODO -- ??)

         -- add segment
         points_used[#points_used+1] = np
         river_pts[#river_pts+1] = np
         self.__all_river_points[#self.__all_river_points+1] = np

         if #river_pts > 100 then break end -- avoid infinite loops

         -- the point found is now the current point
         p = np
      end

      ::done::

      -- do not accept rivers of length 6
      if #river_pts < 6 then 
         goto try_again 
      end

      -- add river
      self.rivers[#self.rivers+1] = river_pts
   end
 */
}


double 
MapGen::PointAltitude(Point const& p) const
{
	double prop_w = rect.P1().X() / (rect.P2().X() - rect.P1().X()),
	       prop_h = rect.P1().Y() / (rect.P2().Y() - rect.P1().Y());
	int prop_x = (p.X() / (rect.P2().X() - rect.P1().X()) - prop_w) * 255,
	    prop_y = (p.Y() / (rect.P2().Y() - rect.P1().Y()) - prop_h) * 255;
	return hm[prop_x][prop_y];
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
	return (static_cast<double>(rand_r(&seedp)) / static_cast<double>(RAND_MAX));
}


Point
MapGen::RandomPoint() const
{
	return Point(Random() * (-rect.P1().X()+rect.P2().X()) + rect.P1().X(),
		     Random() * (-rect.P1().Y()+rect.P2().Y()) + rect.P1().Y());
}


void 
MapGen::RandomOffcentre(int& x, int& y, double& r) const
{
	r = Random() * 24 + 6;
	double theta = Random() * 2 * M_PI + Random();
	double distance = Random() * (128 - r * 2);
	x = floor(128 + cos(theta) * distance);
	y = floor(128 + sin(theta) * distance);
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
