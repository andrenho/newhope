#include "physics.h"

cpSpace* space;

int physics_init(lua_State* L)
{
	space = cpSpaceNew();
	return 0;
}


int physics_finish(lua_State* L)
{
	cpSpaceFree(space);
	return 0;
}
