#include "engine/city.h"

#include <stdlib.h>

#include "engine/building.h"

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
