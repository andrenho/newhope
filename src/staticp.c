#include "staticp.h"

#include <lauxlib.h>
#include <chipmunk.h>

#include "luah.h"

extern cpSpace* space;

int staticp_init(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TTABLE);

	// get object fields
	cpFloat x, y;
	LUA_FIELD(L, x, "x", number);
	LUA_FIELD(L, y, "y", number);
	
	// create person body
	cpBody* body = cpSpaceAddBody(space, cpBodyNew(INFINITY, INFINITY));
	cpBodySetPos(body, cpv(x, y));

	// create person shape
	cpShape* shape = cpSpaceAddShape(space, 
			cpCircleShapeNew(body, 0.5, cpvzero));
	cpShapeSetFriction(shape, 0.1);

	// save pointers to LUA
	LUA_SET_FIELD(L, body, "body", lightuserdata);
	LUA_SET_FIELD(L, shape, "shape", lightuserdata);

	return 0;
}


int staticp_clean_up(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TTABLE);

	cpBody* body;
	cpShape* shape;
	LUA_FIELD(L, body, "body", userdata);
	LUA_FIELD(L, shape, "shape", userdata);

	cpSpaceRemoveShape(space, shape);
	cpSpaceRemoveBody(space, body);
	cpShapeFree(shape);
	cpBodyFree(body);

	return 0;
}
