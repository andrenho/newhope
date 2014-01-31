// Copyright 2014 <Imperial Software>

#ifndef ENGINE_MAPGEN_H_
#define ENGINE_MAPGEN_H_

#include <boost/polygon/voronoi.hpp>
#include <random>

#include "engine/rectangle.h"

class MapArea {

};

class MapGen {
public:
	MapGen(int x1, int y1, int x2, int y2);
	MapGen(int x1, int y1, int x2, int y2, unsigned int seed);

	void Create();

	virtual ~MapGen();

private:
	void CreateVoronoi(int points);

	const Rectangle rect;
	boost::polygon::voronoi_diagram<double> vd;
	std::default_random_engine generator;
};

#endif  // ENGINE_MAPGEN_H_
