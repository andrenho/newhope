#include "engine/world.h"

#include <stdlib.h>

World* world_init()
{
	World* w = malloc(sizeof(World));
	return w;
}


void world_free(World** w)
{
	free(*w);
	*w = NULL;
}
