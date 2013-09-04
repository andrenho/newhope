#include "engine/building.h"

#include <stdlib.h>

Building* building_init(int x, int y)
{
	Building* b = calloc(sizeof(Building), 1);
	b->x = x;
	b->y = y;
	return b;
}


void building_free(Building** b)
{
	free(*b);
	*b = NULL;
}
