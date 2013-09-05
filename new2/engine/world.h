#ifndef ENGINE_WORLD_H
#define ENGINE_WOLRD_H

#include <stdbool.h>

#include "engine/terrain.h"
#include "engine/person.h"
#include "engine/object.h"
#include "util/list.h"

typedef struct World {
	int w, h;
	Person* hero;
	List* people;
	List* cities;
} World;

World* world_init(int w, int h);
void world_free(World** w);

void world_step(World* w);
Terrain world_xy(World* w, int x, int y, Object* obj);
bool world_tile_walkable(World* w, int x, int y);

#endif
