#include "physics.h"

#include <lauxlib.h>

#include "luah.h"

cpSpace* space;

static int collision_callback(cpArbiter *arb, struct cpSpace *space, void *data)
{
	cpBody *a, *b;

	cpArbiterGetBodies(arb, &a, &b);

	// ingnore static objects (TODO)
	if(a == space->staticBody || b == space->staticBody)
		return 1;

	// call both objects 'collision'
	lua_State* L = (lua_State*)data;
	lua_getglobal(L, "world");
	LUA_PUSH_METHOD(L, "collision_callback");
	lua_pushlightuserdata(L, a);
	lua_pushlightuserdata(L, b);
	luah_call(L, 3, 0);

	return 1;
}


int physics_init(lua_State* L)
{
	space = cpSpaceNew();
	cpSpaceSetDefaultCollisionHandler(space, collision_callback, 
			NULL, NULL, NULL, L);
	return 0;
}


int physics_step(lua_State* L)
{
	cpSpaceStep(space, 1.0/60.0);
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


static void free_static_shape(cpBody *body, cpShape *shape, void* data)
{
	cpSpaceRemoveShape(space, shape);
	cpShapeFree(shape);
}


int physics_finish(lua_State* L)
{
	cpBodyEachShape(space->staticBody, free_static_shape, NULL);
	cpSpaceFree(space);
	return 0;
}


int physics_obj_pos(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	
	// get data
	cpBody* body;
	LUA_FIELD(L, body, "body", userdata);
	cpVect pos = cpBodyGetPos(body);
	cpFloat angle = cpBodyGetAngle(body);

	// create table
	lua_createtable(L, 0, 3);
	LUA_SET_FIELD(L, pos.x, "x", number);
	LUA_SET_FIELD(L, pos.y, "y", number);
	LUA_SET_FIELD(L, angle, "angle", number);

	return 1;
}
