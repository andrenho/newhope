#ifndef ENGINE_BUILDING_H
#define ENGINE_BUILDING_H

typedef struct {
	int x, y;
} Building;

Building* building_init(int x, int y);
void building_free(Building** b);

#endif
