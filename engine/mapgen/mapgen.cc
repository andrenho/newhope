#include "engine/mapgen/mapgen.h"

#include <cfloat>
#include <cmath>
#include <ctime>

#include <algorithm>
#include <iostream>
using namespace std;

#ifndef M_PI
#  define M_PI 3.14159265358979323846
#endif

#include "./main.h"
#include "engine/block.h"
#include "engine/point.h"
#include "engine/rectangle.h"
#include "engine/world.h"
#include "engine/mapgen/rivergen.h"
#include "engine/mapgen/roadgen.h"

MapGen::MapGen(int x1, int y1, int x2, int y2)
    : MapGen(x1, y1, x2, y2, static_cast<unsigned int>(time(0)))
{
}


MapGen::MapGen(int x1, int y1, int x2, int y2, unsigned int seed)
    : rect(Rectangle_(Point(x1, y1), Point(x2, y2))), seedp(seed),
      points(), data(), tile_cache()
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
    rivergen = unique_ptr<Rivergen>(new Rivergen(hm, rect, seedp));
    for(int i=0; i<12; i++) {
        rivers.push_back(rivergen->CreateRiver());
    }
    roadgen = unique_ptr<Roadgen>(new Roadgen(rect, seedp));

    // find biomes
    CalculateMoisture();
    CreateBiomes();
    CreateBeaches();
}


void 
MapGen::CreateRoads(vector<class City>& cities)
{
    // create roads
    roads = roadgen->CreateRoads(cities);
}


Block
MapGen::Terrain(int x, int y) const
{
    Point pt(x, y);
    unordered_map<Point,Block>::const_iterator it;
    if((it = tile_cache.find(pt)) != tile_cache.end()) {
        return it->second;
    } else if(rivergen->TileInSegment(x, y)) {
        tile_cache[Point(x,y)] = Block::WATER;
        return Block::WATER;
    } else if(roadgen->TileInSegment(x, y)) {
        tile_cache[Point(x,y)] = Block::ASPHALT;
        return Block::ASPHALT;
    } else {
        Point p = ClosestPoint(x, y);
        Block b = data.at(p).Biome;
        tile_cache[Point(x,y)] = b; // XXX - remove to deactivate cache
        return b;
    }
}


unordered_set<Point> 
MapGen::CitiesPositions(unsigned int n) const
{
    unordered_set<Point> positions;

    while(positions.size() < n) {
        for(auto const& biome : world->Blocks.TerrainList()) {
            if(biome == Block::WATER) {
                continue;
            }
            if(positions.size() == n) {
                break;
            }
            Point p(0, 0);
            if(RandomPointWithBiome(p, biome, positions)) {
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
            min_distance = min(pt.Distance(p), min_distance);
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


bool 
MapGen::RandomPointWithBiome(Point& pt, Block biome, 
        unordered_set<Point> ignore) const
{
    // ignore == cities already present

    // create vector only with points in biome, and not ignored
    vector<Point> pts;
    for(auto const& p : points) {
        int x = static_cast<int>(p.X()),
            y = static_cast<int>(p.Y());
        if(Terrain(x, y) == biome) {
            // not too close to the other points
            double min_distance = DBL_MAX;
            for(auto const& pos : ignore) {
                min_distance = min(min_distance, pos.Distance(p));
            }
            if(min_distance > 2000) {
                pts.push_back(p);
            }
        }
    }

    if(pts.empty()) {
        return false;
    } else {
        // choose a random point
        double r = world->Random();
        unsigned int idx = static_cast<unsigned int>(r * static_cast<double>(pts.size()));
        pt = pts[idx];
        return true;
    }
}


void 
MapGen::RandomOffcentre(int& x, int& y, double& r)
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


// vim: ts=4:sw=4:sts=4:expandtab
