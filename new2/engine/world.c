#include "engine/world.h"

#include <stdlib.h>

#include "engine/city.h"
#include "util/geometry.h"


static Object world_object_xy(World* w, int x, int y);


World* world_init(int _w, int _h)
{
	World* w = calloc(sizeof(World), 1);
	w->w = _w;
	w->h = _h;
	w->hero = person_init(1, 1);
	w->people = list();
	w->cities = list();

	add(&w->people, w->hero);
	add(&w->cities, city_init("Test", 10, 10, 10, 10));

	person_set_as_hero(w->hero);

	return w;
}


void world_free(World** w)
{
	free_all(&(*w)->cities, city_free);
	free_all(&(*w)->people, person_free);
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
	if(obj)
		(*obj) = world_object_xy(w, x, y);

	// tile
	if(x < 0 || y < 0 || x >= w->w || y >= w->h)
		return OCEAN;
	if(x == 10 && y == 10)
		return OCEAN;

	return GRASS;
}


bool world_tile_walkable(World* w, int x, int y)
{
	Object obj;
	Terrain t = world_xy(w, x, y, &obj);

	// tiles
	if(t == OCEAN) {
		return false;
	}

	// objects
	if(obj.type == WALL) {
		return false;
	}

	return true;
}


/********************
 *                  *
 * STATIC FUNCTIONS *
 *                  *
 ********************/


static Object world_object_xy(World* w, int x, int y)
{
	Object obj = { NONE, 0 };

	// check for buildings
	FOREACH(w->cities, City*, c) {
		Rect r = { c->x, c->y, c->w, c->h };
		Point p = { x, y };
		if(intersect_point(r, p)) {
			return city_object_xy(c, x, y);
		}
	}

	return obj;
}
