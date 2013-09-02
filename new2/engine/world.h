#ifndef ENGINE_WORLD_H
#define ENGINE_WOLRD_H

#include "engine/terrain.h"
#include "engine/person.h"
#include "util/list.h"

typedef enum Object {
	NONE,
} Object;

typedef struct World {
	int w, h;
	List* people;
	Person* hero;
} World;

World* world_init(int w, int h);
void world_free(World** w);

void world_step(World* w);
Terrain world_xy(World* w, int x, int y, Object* obj);

#endif
