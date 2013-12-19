#include "physics.h"

#include <lauxlib.h>

#include "luah.h"

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


int physics_create_static_obj(lua_State* L)
{
	cpFloat x = luaL_checknumber(L, 2),
		y = luaL_checknumber(L, 3),
		w = luaL_checknumber(L, 4),
		h = luaL_checknumber(L, 5);

	cpShape *shape = cpBoxShapeNew2(space->staticBody, 
			cpBBNew(x-(w/2), y-(h/2), x+(w/2), y+(h/2)));
	cpShapeSetFriction(shape, 1);
	cpSpaceAddShape(space, shape);

	return 0;
}
