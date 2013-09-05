#include "engine/car.h"

#include <stdlib.h>

Car* car_init(int x, int y)
{
	Car* c = calloc(sizeof(Car), 1);
	c->x = x;
	c->y = y;
	return c;
}


void car_free(Car** c)
{
	free(*c);
	*c = NULL;
}
