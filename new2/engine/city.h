#ifndef ENGINE_CITY_H
#define ENGINE_CITY_H

typedef struct City {
	const char* name;
	int x, y, w, h;
} City;

City* city_init(const char* name, int x, int y, int w, int h);
void city_free(City** c);

#endif
