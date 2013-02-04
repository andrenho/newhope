#include "world/mapbuild.h"

#include <cassert>
#include <algorithm>
#include <vector>
#include <set>
using namespace std;

#include "util/logger.h"
#include "util/polygon.h"
#include "world/biome.h"
#include "world/city.h"

MapBuild::MapBuild(const MapParameters& pars)
	: pars_(pars)
{
	CreatePolygons();
	CreateCoastline();
	CreateLakes();
	CreateElevation();
	CreateRivers();
	CreateMoisture();
	CreateLava();
	CreateBiomes();
	CreateCities();
	CreateRoads();
}


MapBuild::~MapBuild()
{
	for(const auto& city: cities_) {
		delete city;
	}
	for(const auto& river: rivers_) {
		delete river;
	}
	for(const auto& lava_path: lava_) {
		delete lava_path;
	}
	for(const auto& road: roads_) {
		delete road;
	}
	for(const auto& biome: biomes_) {
		delete biome;
	}
}


void
MapBuild::CreatePolygons()
{
	logger.Debug("Generating map polygons...");

	// create layout
	vector<Polygon*> polygons;
	Polygon::FakeVoronoi(pars_.seed, pars_.w, pars_.h, 60, polygons);

	// add biomes_
	for(auto it(polygons.begin()); it != polygons.end(); it++)
		biomes_.push_back(new Biome(*it));
}


void
MapBuild::CreateCoastline()
{
	logger.Debug("Generating map coastline...");

	// middle square
	int qw(pars_.w / 8),
	    qh(pars_.h / 8);
	Point<int> pts[] { { qw*2, qh*2 }, { qw*6, qh*2 }, 
		           { qw*6, qh*6 }, { qw*2, qh*6 } };

	// disturb points
	int max_disturb(qw);
	for(int i(0); i<4; i++) {
		pts[i].x += (rand() % max_disturb) - max_disturb/2;
		pts[i].y += (rand() % max_disturb) - max_disturb/2;
	}
	
	// create coastline
	Polygon polygon(pts, 4);
	polygon.MidlineDisplacement(4);
	for(const auto& biome: biomes_) {
		Point<int> p(biome->polygon().Midpoint());
		if(!polygon.PointInPolygon(p)) {
			biome->set_terrain(t_WATER);
		}
	}
}


void
MapBuild::CreateLakes()
{
	// add lakes
	logger.Debug("Adding lakes...");
	for(int i(0); i<rand()%2+3; i++) {
		Rect r(rand() % 3000 + 800, rand() % 3000 + 800,
				rand() % 500 + 100, rand() % 500 + 100);
		Polygon lake(r);
		lake.MidlineDisplacement(2);
		for(const auto& biome : biomes_) {
			if(lake.PointInPolygon(biome->polygon().Midpoint())) {
				biome->set_terrain(t_WATER);
			}
		}
	}
}


void
MapBuild::CreateElevation()
{
	logger.Debug("Generating map elevation...");

	// set water elevation
	for(const auto& biome : biomes_) {
		if(biome->terrain() == t_WATER) {
			for(const auto& point : biome->polygon().points()) {
				point.elevation = -1;
			}
			biome->set_elevation(-1);
		}
	}

	// set other tiles elevation
	int max_alt(0);
	for(const auto& biome : biomes_) {
		if(biome->terrain() != t_WATER)
		{
			int tot_alt(0);
			for(const auto& point: biome->polygon().points()) {
				point.elevation = DistanceFromWater(
						point, false);
				point.elevation += rand() % 200;
				tot_alt += point.elevation;
				max_alt = max(max_alt, point.elevation);
			}
			biome->set_elevation(tot_alt / 
					biome->polygon().points().size());
		}
	}
	
	// normalize elevation
	for(const auto& biome : biomes_) {
		if(biome->elevation() != -1) {
			biome->set_elevation((double)biome->elevation() /
				             (double)max_alt * (double)100);
		}
	}
}


void
MapBuild::CreateRivers()
{
	logger.Debug("Generating rivers_...");

	int rivers__left(pars_.n_rivers);

	while(rivers__left > 0) {
		int b(rand() % biomes_.size());
		if(biomes_[b]->terrain() != t_WATER) {
			int k(rand() % biomes_[b]->polygon().points().size());
			Point<int> p(biomes_[b]->polygon().points()[k]);
			if(DistanceFromWater(p, false) > 600) {
				rivers_.push_back(CreateFlow(p));
				--rivers__left;
			}
		}
	}
}


void 
MapBuild::CreateMoisture()
{
	logger.Debug("Generating moisture...");

	// generate moisture
	int max_moi(0);
	for(const auto& biome : biomes_)
	{
		if(biome->terrain() == t_WATER) {
			biome->set_moisture(0);
		} else {
			int tot_moi(0);
			for(const auto& point : biome->polygon().points()) {
				int moi(DistanceFromWater(point, true));
				tot_moi += moi;
				max_moi = max(max_moi, moi);
			}
			biome->set_moisture(tot_moi / 
					    biome->polygon().points().size());
		}
	}
	
	// normalize moisture
	for(const auto& biome : biomes_) {
		biome->set_moisture(100 - ((double)biome->moisture() /
			                   (double)max_moi * (double)100));
	}
}


void
MapBuild::CreateLava()
{
	logger.Debug("Creating lava_...");

	// find highest, dryest point
	vector<const Biome*> bs;
	copy(biomes_.begin(), biomes_.end(), back_inserter(bs));
	sort(bs.begin(), bs.end(), 
	[](const Biome* const& b1, const Biome* const& b2) -> bool
		{
			int i1((100 - b1->elevation()) + b1->moisture());
			int i2((100 - b2->elevation()) + b2->moisture());
			return i1 < i2;
		});

	// draw lava_ path
	for(int i=0; i<3; i++) {
		auto points(bs[i]->polygon().points());
		lava_.push_back(CreateFlow(points[rand() % points.size()], 4));
	}
}


void
MapBuild::CreateBiomes()
{
	/*
	 * Elev/Moist   0-30       30-60      60-100
	 *  0-30        DIRT       EARTH      GRASS
	 * 30-60        ROCK       GRASS      HOTFOREST
	 * 60-100       LAVAROCK   SNOW       COLDFOREST
	 */
	for(const auto& biome : biomes_) {
		if(biome->terrain() == t_WATER) {
			continue;
		}

		int alt = biome->elevation();
		int moi = biome->moisture();
		TerrainType t = t_DIRT;
		if(alt < 20) {
			t = t_DIRT;
		} else if(alt < 40) {
			if(moi < 30)
				t = t_DIRT;
			else if(moi < 60)
				t = t_EARTH;
			else
				t = t_GRASS;
		} else if(alt < 70) {
			if(moi < 30)
				t = t_ROCK;
			else if(moi < 60)
				t = t_GRASS;
			else
				t = t_HOTFOREST;
		} else {
			if(moi < 60)
				t = t_LAVAROCK;
			else if(moi < 80)
				t = t_SNOW;
			else
				t = t_COLDFOREST;
		}
		biome->set_terrain(t);
	}
}


void
MapBuild::CreateCities()
{
	logger.Debug("Creating cities_...");

	for(int i(0); i<pars_.n_cities; i++) {
try_again:
		int b(rand() % biomes_.size());
		
		// check if it's not in the sea
		if(biomes_[b]->terrain() == t_WATER) {
			goto try_again;
		}

		// check if neighbours don't have a city already
		vector<const Biome*> neighbours;
		BiomeNeighbours(*biomes_[b], neighbours);
		for(const auto& neigh: neighbours) {
			if(neigh->has_city()) {
				goto try_again;
			}
		}

		// add city
		Point<int> pos(biomes_[b]->polygon().Midpoint());
		cities_.push_back(new City(pos, *biomes_[b], CityStyle::VICTORIAN));
		biomes_[b]->set_has_city(true);
	}
}


void
MapBuild::CreateRoads()
{
	logger.Debug("Creating roads_...");

	for(const auto& city: cities_) {
		// find closest city
		vector<City*> cs;
		copy(cities_.begin(), cities_.end(), back_inserter(cs));
		sort(cs.begin(), cs.end(), 
			[&city](City* const& c1, City* const& c2) -> bool
			{ 
				int d1 = c1->pos().Distance(city->pos());
				int d2 = c2->pos().Distance(city->pos());
				return d1 < d2;
			});

		// skip cities_ already connected
		unsigned int n; // [0] is the city itself
		for(n=1; ; n++) {
			if(n >= cs.size()) {
				goto next;
			}
			auto conn = cs[n]->connections();
			if(find(conn.begin(), conn.end(), city) == conn.end()) {
				break;
			}
		}

		// build road
		CreateRoad(*city, *cs[n]);
		
		// add connections
		cs[n]->AddConnection(city);
		city->AddConnection(cs[n]);

next: ;
	}

	FindUnconnectedCities();
}


void MapBuild::FindUnconnectedCities()
{
	// find city clusters
	vector<vector<const City*> > clusters;
	for(const auto& city: cities_) {
		bool found(false);
		for(const auto& cluster: clusters) {
			if(find(cluster.begin(), cluster.end(), city) !=
					cluster.end())
			{
				found = true;
				break;
			}
		}
		if(found) {
			continue;
		}

		auto cluster = vector<const City*>();
		FindCityCluster(city, cluster);
		clusters.push_back(cluster);
	}
	
	// find closest cities_ and build roads_ among them
	for(unsigned int i(0); i<clusters.size(); i++) {
		for(unsigned int j(i+1); j<clusters.size(); j++) {
			ConnectClusters(clusters[i], clusters[j]);
		}
	}
}


void 
MapBuild::FindCityCluster(const City* city, vector<const City*>& cluster)
{
	if(find(cluster.begin(), cluster.end(), city) != cluster.end()) {
		return;
	}

	cluster.push_back(city);
	for(const auto& conn: city->connections()) {
		FindCityCluster(conn, cluster); // recursive
	}
}


void
MapBuild::ConnectClusters(vector<const City*> const& c1, 
		vector<const City*> const& c2)
{
	int min_dist(INT_MAX);
	pair<const City*, const City*> cities_(nullptr, nullptr);
	for(unsigned int i(0); i<c1.size(); i++) {
		for(unsigned int j(i+1); j<c2.size(); j++) {
			if(c1[i] != c2[j]) {
				int dist(c1[i]->pos().Distance(c2[j]->pos()));
				if(dist < min_dist) {
					min_dist = dist;
					cities_ = make_pair(c1[i], c2[j]);
				}
			}
		}
	}
	if(cities_.first) {
		CreateRoad(*cities_.first, *cities_.second);
	}
}


/*
void
MapBuild::FindNations()
{
	logger.Debug("Creating nations...");

	vector<vector<const City*> > nations;
	for(const auto& city: cities_)
	{
		bool found = false;
		for(const auto& nation: nations)
			if(find(nation.begin(), nation.end(), city) !=
					nation.end())
			{
				found = true;
				break;
			}
		if(found)
			continue;

		auto nation = vector<const City*>();
		FindInterconnectedCities(*city, nation);
		nations.push_back(nation);
	}
}


void 
MapBuild::FindInterconnectedCities(const City& city, 
		vector<const City*>& cs)
{
	// TODO
}
*/


void 
MapBuild::CreateRoad(const City& c1, const City& c2)
{
	if(&c1 == &c2) {
		return;
	}

	const Biome* b(&c1.biome());
	Polygon* road(new Polygon());

	// create road points
	while(b != &c2.biome()) {

		road->AddPoint(b->polygon().Midpoint());

		// find neighbour biomes_
		vector<const Biome*> biomes_;
		BiomeNeighbours(*b, biomes_);

		// find closest biome
		sort(biomes_.begin(), biomes_.end(), 
			[&c2](const Biome* const& b1, const Biome* const& b2) -> bool
			{ 
				int d1(b1->polygon().Midpoint().Distance(
					c2.biome().polygon().Midpoint()));
				int d2(b2->polygon().Midpoint().Distance(
					c2.biome().polygon().Midpoint()));
				if(b1->terrain() == t_WATER) {
					d1 = INT_MAX;
				}
				if(b2->terrain() == t_WATER) {
					d2 = INT_MAX;
				}
				return d1 < d2;
			});
		
		// next biome
		road->AddPoint(b->polygon().Midpoint());
		b = biomes_[0];
	}

	// add roads_
	road->AddPoint(c2.biome().polygon().Midpoint());
	road->CalculateLimits();
	roads_.push_back(road);
}


Polygon*
MapBuild::CreateFlow(Point<int> start, int iterations)
{
	Polygon* poly(new Polygon());
	Point<int> p(start);
	int iter(0);

	while(p.elevation != -1 && iter < iterations) {

		poly->AddPoint(p);
		
		// find neighbours
		vector<Point<int>> neighbours;
		for(const auto& biome : biomes_) {
			if(biome->polygon().ContainsPoint(p)) {
				biome->polygon().NeighbourPoints(p, neighbours);
			}
		}
		assert(neighbours.size() > 0);

		// order by elevation
		sort(neighbours.begin(), neighbours.end(), 
		[](Point<int> const& p1, Point<int> const& p2) -> bool
		{ 
			return p1.elevation < p2.elevation; 
		});

		// skip points already added
		unsigned int n(0);
		while(true) {
			p = neighbours[n];
			if(find(poly->points().begin(), poly->points().end(), p) == 
					poly->points().end()) {
				break;
			}
			if(n >= neighbours.size()) {
				return poly;
			}
			++n;
		}

		--iter;
	}
	poly->CalculateLimits();
	return poly;
}


int 
MapBuild::DistanceFromWater(Point<int> const& p, bool include_rivers_)
{
	int dist = INT_MAX;

	// distance from sea
	for(const auto& biome : biomes_) {
		if(biome->terrain() == t_WATER) {
			int new_dist(p.Distance(biome->polygon().Midpoint()));
			if(new_dist < dist) {
				dist = new_dist;
			}
		}
	}

	// distance from a river
	if(include_rivers_) {
		for(const auto& river : rivers_) {
			for(const auto& point : river->points()) {
				int new_dist(point.Distance(p));
				if(new_dist < dist) {
					dist = new_dist;
				}
			}
		}
	}

	assert(dist != INT_MAX);
	return dist;
}


bool 
MapBuild::AreNeighbours(const Biome& b1, const Biome& b2)
{
	vector<const Biome*> neigh;
	BiomeNeighbours(b1, neigh);
	return find(neigh.begin(), neigh.end(), &b2) != neigh.end();
}


void 
MapBuild::BiomeNeighbours(const Biome& biome, vector<const Biome*>& biomes_)
{
	for(const auto& b: this->biomes_) {
		if(biome.polygon().IsTouching(b->polygon()) && &biome != b) {
			biomes_.push_back(b);
		}
	}
}
