#include "world/world.h"

#include <algorithm>
#include <cstdlib>
#include <vector>
using namespace std;

#include "util/logger.h"
#include "util/mapcache.h"
#include "util/polygon.h"
#include "world/biome.h"
#include "world/city.h"
#include "world/mapbuild.h"
#include "world/person.h"

World::World(int w, int h) :
	w_(w), h_(h)
{
	logger.Debug("Building map_...");
	MapParameters pars {
		.seed = 2,
		.w = w_,
		.h = h_,
		.n_rivers = 15,
		.n_cities = 1,
		.n_roads = 10,
	};
	map_ = new MapBuild(pars);
	logger.Debug("Map built.");

	// create vectors with rivers, lava and roads
	CreateCaches();

	// add people
	people_.push_back(new Person(*this, map_->cities()[0]->pos()));

	// create terrain cache
	cache = new mapcache<Tile,TerrainType>(4000, TerrainCache, this);

	// randomize terrain
	for(int i(0); i<100; i++) {
		for(int j(0); j<100; j++) {
			randspecial[i][j] = rand() % 100;
		}
	}
}


World::~World()
{
	delete cache;
	for(const auto& person: people_) {
		delete person;
	}
	delete map_;
	logger.Debug("World deleted.");
}


void
World::Process()
{
	for(auto& person: people_) { // TODO - only visible
		person->Process();
	}
}


TerrainType 
World::Terrain(Tile p, bool use_cache) const
{
	if(use_cache) {
		return (*cache)[p];
	} else {
		return TerrainCache((void*)this, p);
	}
}


TerrainType
World::FindBiome(World* ths, Tile p)
{
	// find biome
	unsigned int sz(ths->map_->biomes().size());
	for(unsigned int i(0); i<sz; i++) {
		if(ths->map_->biomes()[i]->polygon().PointInPolygon(p)) {
			return ths->map_->biomes()[i]->terrain();
		}
	}
	return t_WATER;
}


TerrainType 
World::TerrainCache(void* obj, Tile p)
{
	World* ths((World*)obj);

	if(ths->IsParkingLot(p)) {
		p.Debug();
		return t_ROAD;
	}

	if(binary_search(ths->roadpts.begin(), ths->roadpts.end(), p))
	{
		if(FindBiome(ths, p) != t_LAVAROCK)
			return t_ROAD;
		else
			return t_DIRT;
	}
	if(binary_search(ths->riverpts.begin(), ths->riverpts.end(), p)) {
		return t_RIVER;
	}
	if(binary_search(ths->lavapts.begin(), ths->lavapts.end(), p)) {
		return t_LAVA;
	}

	return FindBiome(ths, p);
}


int
World::Special(Tile p) const
{
	return randspecial[(int)p.x % 100][(int)p.y % 100];
}


TreeType 
World::Tree(Tile p) const
{
	if(!(((int)p.x + (int)p.y) % 2))
		return NO_TREE;

	const int DENSITY_NO_FOREST = 99,
	          DENSITY_FOREST = 40;
	TerrainType tr = Terrain(p);
	if(tr == t_SNOW || tr == t_TUNDRA) {
		if(Special(p) >= DENSITY_NO_FOREST) {
			return TREE_POINTY;
		}
	} else if(tr == t_COLDFOREST) {
		if(Special(p) >= DENSITY_FOREST) {
			return TREE_POINTY;
		}
	} else if(tr == t_GRASS) {
		if(Special(p) >= DENSITY_NO_FOREST) {
			return TREE_ROUND;
		}
	} else if(tr == t_HOTFOREST) {
		if(Special(p) >= DENSITY_FOREST) {
			return TREE_ROUND;
		}
	}
	return NO_TREE; // TODO - add desert trees
}


bool 
World::TreeSmall(Tile p) const
{
	return Special(p) < 50;
}


void 
World::CreateCaches()
{
	logger.Debug("Drawing polygons...");

	static struct {
		vector<Polygon<Tile>*> const& map_build;
		vector<Tile>& points;
		int width;
	} polygons[] {
		{ map_->roads(),  roadpts,  6 },
		{ map_->rivers(), riverpts, 5 },
		{ map_->lava(),   lavapts,  2 },
	};

	// road, rivers and lava
	for(const auto& polygon: polygons) {
		// create a set with all the points
		set<Tile> points;
		for(const auto& each: polygon.map_build) {
			for(unsigned int i=0; i<each->points().size()-1; i++) {
				AddPoints(each->points()[i], each->points()[i+1],
						points, polygon.width);
			}
		}

		// transform the set in a vector and sort it
		copy(points.begin(), points.end(), back_inserter(polygon.points));
		sort(polygon.points.begin(), polygon.points.end());
	}

	// cities
	for(const auto& city: map_->cities()) {
		city_rects_[city] = city->Limits();
	}
	
	logger.Debug("Polygons drawn.");
}


void
World::AddPoints(Tile p1, Tile p2, set<Tile>& points, 
		int line_width)
{
	int x0(min(max(p1.x, 0.0), double(w_ - line_width))),
	    y0(min(max(p1.y, 0.0), double(h_ - line_width))),
	    x1(min(p2.x, double(w_ - line_width))),
	    y1(min(p2.y, double(h_ - line_width)));
	int dx(abs(x1-x0)), sx(x0<x1 ? 1 : -1);
	int dy(abs(y1-y0)), sy(y0<y1 ? 1 : -1);
	int err((dx>dy ? dx : -dy)/2), e2;

	for(;;) {
		int xx(0);
		int yy(0);
		for(int x(-line_width/2); x<line_width/2; x++) {
			for(int y(-line_width/2); y<line_width/2; y++) {
				points.insert(Tile(x0+x+xx, y0+y+yy));
			}
		}
		if(x0 == x1 && y0 == y1) {
			break;
		}
		e2 = err;
		if(e2 > -dx) {
			err -= dy;
			x0 += sx;
		}
		if(e2 < dy) {
			err += dx;
			y0 += sy;
		}
	}
}


const City* 
World::CityOnTile(Tile t) const
{
	for(auto const& kv: city_rects_) {
		if(kv.second.ContainsPoint(t)) {
			return kv.first;
		}
	}
	return nullptr;
}


bool 
World::IsParkingLot(Tile p) const
{
	const City* city(CityOnTile(p));
	if(city) {
		for(auto const& b: city->buildings()) {
			if(b->ParkingLot().ContainsPoint(p)) {
				b->ParkingLot().Debug();
				return true;
			}
		}
	}
	return false;
}
