#include "engine/building.h"

#include <stdlib.h>

Building* building_init(int x, int y, BuildingClass class, int layout)
{
	Building* b = calloc(sizeof(Building), 1);
	b->x = x;
	b->y = y;
	b->class = class;
	b->layout = layout;
	return b;
}


void building_free(Building** b)
{
	free(*b);
	*b = NULL;
}


BuildingLayout building_layout(Building* b)
{
	return bd_type[b->class].layout[b->layout];
}


Object building_object_xy(Building* b, int x, int y)
{
	BuildingLayout bl = building_layout(b);

	// TODO - doors

	// building walls
	if(x == 0) {
		if(y == 0) {
			return (Object) { WALL, WALL_NW };
		} else if(y == bl.h-1) {
			return (Object) { WALL, WALL_SW };
		} else {
			return (Object) { WALL, WALL_W };
		}
	} else if(x == bl.w-1) {
		if(y == 0) {
			return (Object) { WALL, WALL_NE };
		} else if(y == bl.h-1) {
			return (Object) { WALL, WALL_SE };
		} else {
			return (Object) { WALL, WALL_E };
		}
	} else if(y == 0) {
		return (Object) { WALL, WALL_N };
	} else if(y == bl.h-1) {
		return (Object) { WALL, WALL_S };
	}

	return (Object){ NONE, 0 };
}
