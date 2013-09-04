#include "engine/city.h"

#include <stdlib.h>

City* city_init(const char* name, int x, int y, int w, int h)
{
	City* c = calloc(sizeof(City), 1);
	c->name = name;
	c->x = x; c->y = y; c->w = w; c->h = h;
	return c;
}


void city_free(City** c)
{
	free(*c);
	*c = NULL;
}
