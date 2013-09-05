#ifndef ENGINE_CITY_H
#define ENGINE_CITY_H

#include "engine/object.h"
#include "util/list.h"

typedef struct City {
	const char* name;
	int x, y, w, h;
	List* buildings;
} City;

City* city_init(const char* name, int x, int y, int w, int h);
void city_free(City** c);

Object city_object_xy(City* c, int x, int y);

#endif
