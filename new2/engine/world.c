#include "engine/world.h"

#include <stdlib.h>

World* world_init(int _w, int _h)
{
	World* w = malloc(sizeof(World));
	w->w = _w;
	w->h = _h;
	return w;
}


void world_free(World** w)
{
	free(*w);
	*w = NULL;
}


Terrain world_xy(World* w, int x, int y, Object* obj)
{
	*obj = NONE;

	if(x < 0 || y < 0 || x >= w->w || y >= w->h)
		return OUT_OF_BOUNDS;

	return GRASS;
}
