#ifndef ENGINE_BUILDING_H
#define ENGINE_BUILDING_H

#include "engine/buildingtype.h"
#include "engine/object.h"

typedef struct {
	int x, y;
	BuildingClass class;
	int layout;
} Building;

Building* building_init(int x, int y, BuildingClass class, int layout);
void building_free(Building** b);

BuildingLayout building_layout(Building* b);
Object building_object_xy(Building* b, int x, int y);

#endif
