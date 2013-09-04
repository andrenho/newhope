#ifndef ENGINE_CITY_H
#define ENGINE_CITY_H

#include "util/list.h"

typedef struct City {
	const char* name;
	int x, y, w, h;
	List* buildings;
} City;

City* city_init(const char* name, int x, int y, int w, int h);
void city_free(City** c);

#endif
