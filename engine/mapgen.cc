// Copyright 2014 <Imperial Software>

#include <vector>

#include "engine/mapgen.h"
#include "engine/point.h"
#include "engine/rectangle.h"

using namespace boost::polygon;

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
	CreateVoronoi(3);
}

/**************************************************************************/

void
MapGen::CreateVoronoi(int points)
{
	std::uniform_int_distribution<int> dx(rect.P1().X(), rect.P2().X()),
		                           dy(rect.P1().Y(), rect.P2().Y());

	std::vector<point_data<int>> pts(points);
	for(int i=0; i<points; i++) {
		int x = dx(generator), y = dy(generator);
		pts.push_back(point_data<int>(x, y));
	}

	construct_voronoi(pts.begin(), pts.end(), &vd);

	int result = 0;
	for (voronoi_diagram<double>::const_vertex_iterator it = vd.vertices().begin(); 
			it != vd.vertices().end(); ++it) {
		const voronoi_diagram<double>::vertex_type &vertex = *it;
		const voronoi_diagram<double>::edge_type *edge = vertex.incident_edge();
		do {
			if (edge->is_primary()) {
				//printf("**\n");
				if(edge->vertex0())
					printf("** %0.2f %0.2f\n", edge->vertex0()->x(), edge->vertex0()->y());
				++result;
			}
			edge = edge->next();
		} while (edge != vertex.incident_edge());
		printf("%0.2f %0.2f\n", vertex.x(), vertex.y());
	}
	printf("%d\n", result);
}
