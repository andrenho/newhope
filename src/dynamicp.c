#include "dynamicp.h"

#include <lauxlib.h>
#include <chipmunk.h>

#include "luah.h"

extern cpSpace* space;

int dynamicp_init(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TTABLE);

	// get object fields
	cpFloat x, y;
	LUA_FIELD(L, x, "x", number);
	LUA_FIELD(L, y, "y", number);
	
	// create person body
	cpBody* body = cpSpaceAddBody(space, cpBodyNew(10, INFINITY));
	cpBodySetPos(body, cpv(x, y));

	// create person shape
	cpShape* shape = cpSpaceAddShape(space, 
			cpCircleShapeNew(body, 0.5, cpvzero));
	cpShapeSetFriction(shape, 0.1);

	// create target body
	cpBody* target = cpBodyNew(INFINITY, INFINITY);
	cpBodySetPos(target, cpv(x, y));

	// create joint
	cpConstraint* joint = cpSpaceAddConstraint(space, 
			cpPivotJointNew2(target, body, cpvzero, cpvzero));
	joint->maxBias = 15.0f;
	joint->maxForce = 30000.0f;

	// save pointers to LUA
	LUA_SET_FIELD(L, body, "body", lightuserdata);
	LUA_SET_FIELD(L, shape, "shape", lightuserdata);
	LUA_SET_FIELD(L, joint, "joint", lightuserdata);
	LUA_SET_FIELD(L, target, "target", lightuserdata);
	
	return 0;
}


int dynamicp_set_target(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	double x = luaL_checknumber(L, 2);
	double y = luaL_checknumber(L, 3);
	lua_pop(L, 2);

	cpBody* target;
	LUA_FIELD(L, target, "target", userdata);
	if(target) {
		cpBodySetPos(target, cpv(x, y));
	}

	return 0;
}


int dynamicp_clean_up(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TTABLE);

	cpBody *body, *target;
	cpShape* shape;
	cpConstraint* joint;
	LUA_FIELD(L, body, "body", userdata);
	LUA_FIELD(L, shape, "shape", userdata);
	LUA_FIELD(L, target, "target", userdata);
	LUA_FIELD(L, joint, "joint", userdata);

	cpSpaceRemoveShape(space, shape);
	cpSpaceRemoveConstraint(space, joint);
	cpSpaceRemoveBody(space, body);

	cpShapeFree(shape);
	cpConstraintFree(joint);
	cpBodyFree(body);
	cpBodyFree(target);

	return 0;
}
