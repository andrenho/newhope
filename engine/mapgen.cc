#include <vector>

#include "engine/mapgen.h"
#include "engine/point.h"
#include "engine/rectangle.h"

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
	CreateVoronoi(2000);
}

/**************************************************************************/

void
MapGen::CreateVoronoi(int npoints)
{
	/*
	std::uniform_int_distribution<int> dx(rect.P1().X(), rect.P2().X()),
		                           dy(rect.P1().Y(), rect.P2().Y());

	std::vector<point_data<int>> pts(npoints);
	for(int i=0; i<npoints; i++) {
		int x = dx(generator), y = dy(generator);
		pts.push_back(point_data<int>(x, y));
	}

	construct_voronoi(pts.begin(), pts.end(), &vd);
	printf("%d %d\n", vd.num_cells(), vd.num_edges());

	int finites = 0;
	std::vector<boost::polygon::point_data<int>> points;
	for(voronoi_diagram<double>::const_cell_iterator it = vd.cells().begin();
			it != vd.cells().end(); ++it) {
		const voronoi_diagram<double>::cell_type& cell = *it;
		const voronoi_diagram<double>::edge_type* edge = cell.incident_edge();
		do {
			if(!edge->is_finite())
				goto next;
			edge = edge->next();
		} while(edge != cell.incident_edge());
		finites++;
next: 0;
	}
	printf("%d\n", finites);
	*/
}
