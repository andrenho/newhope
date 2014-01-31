// Copyright 2014 <Imperial Software>

#include <cstdlib>
#include <ctime>

#include <vector>

#include "engine/mapgen.h"

using namespace boost::polygon;

MapGen::MapGen(int x1, int y1, int x2, int y2)
	: MapGen(x1, y1, x2, y2, time(NULL))
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
	CreateVoronoi();
}

/**************************************************************************/

void
MapGen::CreateVoronoi()
{
	point_data<int> p1(10, 10);
	point_data<int> p2(50, 50);
	point_data<int> p3(10, 50);
	std::vector<point_data<int>> pts = { p1, p2, p3 };
	construct_voronoi(pts.begin(), pts.end(), &vd);

	int result = 0;
	for (voronoi_diagram<double>::const_cell_iterator it = vd.cells().begin(); 
			it != vd.cells().end(); ++it) {
		const voronoi_diagram<double>::cell_type &cell = *it;
		const voronoi_diagram<double>::edge_type *edge = cell.incident_edge();
		do {
			if (edge->is_primary()) {
				++result;
			}
			edge = edge->next();
		} while (edge != cell.incident_edge());
	}
	printf("%d\n", result);
}
