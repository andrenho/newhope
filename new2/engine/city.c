#include "engine/city.h"

#include <stdlib.h>

#include "engine/building.h"
#include "util/geometry.h"

City* city_init(const char* name, int x, int y, int w, int h)
{
	City* c = calloc(sizeof(City), 1);
	c->name = name;
	c->x = x; c->y = y; c->w = w; c->h = h;
	c->buildings = list();

	add(&c->buildings, building_init(2, 2, HOUSE, 0));

	return c;
}


void city_free(City** c)
{
	free_all(&(*c)->buildings, building_free);
	free(*c);
	*c = NULL;
}


Object city_object_xy(City* c, int x, int y)
{
	FOREACH(c->buildings, Building*, b) {
		BuildingLayout bl = building_layout(b);
		Rect r = { (c->x+b->x), (c->y+b->y), bl.w, bl.h };
		Point p = { x, y };
		if(intersect_point(r, p)) {
			return building_object_xy(b, x-(c->x+b->x), y-(c->y+b->y));
		}
	}
	return (Object){ NONE, 0 };
}
