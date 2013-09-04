#include "engine/building.h"

#include <stdlib.h>

Building* building_init(int x, int y, BuildingClass class, int layout)
{
	Building* b = calloc(sizeof(Building), 1);
	b->x = x;
	b->y = y;
	b->class = class;
	b->layout = layout;
	return b;
}


void building_free(Building** b)
{
	free(*b);
	*b = NULL;
}
