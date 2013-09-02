#include "engine/world.h"

#include <stdlib.h>

World* world_init(int _w, int _h)
{
	World* w = calloc(sizeof(World), 1);
	w->w = _w;
	w->h = _h;
	w->hero = person_init(1, 1);
	w->people = list();
	add(&w->people, w->hero);

	return w;
}


void world_free(World** w)
{
	free(*w);
	*w = NULL;
}


void world_step(World* w)
{
	FOREACH(w->people, Person*, p) {
		person_step(p, w);
	}
}


Terrain world_xy(World* w, int x, int y, Object* obj)
{
	*obj = NONE;

	if(x < 0 || y < 0 || x >= w->w || y >= w->h)
		return OUT_OF_BOUNDS;

	return GRASS;
}
