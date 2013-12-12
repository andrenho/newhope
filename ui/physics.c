#include "physics.h"

#include <chipmunk.h>
#include "lua.h"
#include "lauxlib.h"

#include "interface.h"

static cpSpace* space;

void physics_init()
{
	// initialize physics
	cpVect gravity = cpv(0, 0);
	space = cpSpaceNew();
	cpSpaceSetGravity(space, gravity);
}

int cb_add_dynamic_objects(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TTABLE);

	// iterate table
	int n = luaL_len(L, 1);
	for(int i=1; i<=n; i++) {
		cpFloat x, y, w, h, angle, mass, vel_limit;

		// get object
		lua_rawgeti(L, 1, i);

		// get object fields
		LUA_FIELD(x, "x", number);
		LUA_FIELD(y, "y", number);
		LUA_FIELD(w, "w", number);
		LUA_FIELD(h, "h", number);
		LUA_FIELD(angle, "angle", number);
		LUA_FIELD(mass, "mass", number);
		LUA_FIELD(vel_limit, "velocity_limit", number);

		// create chipmunk object
		cpFloat moment = cpMomentForBox(mass, w, h);
		cpBody* body = cpSpaceAddBody(space, cpBodyNew(mass, moment));
		cpBodySetPos(body, cpv(x, y));
		cpBodySetAngle(body, angle);
		cpShape* shape = cpSpaceAddShape(space, cpBoxShapeNew(body, w, h));
		cpShapeSetFriction(shape, 0.7); // TODO ??

		// maximum velocity
		if(vel_limit) {
			cpBodySetVelLimit(body, vel_limit);
		}

		// save pointers
		LUA_SET_FIELD(body, "body", lightuserdata);
		LUA_SET_FIELD(shape, "shape", lightuserdata);

		// pop object
		lua_pop(L, 1);
	}

	return 0;
}


int cb_reset_forces(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TTABLE);

	cpBody* body;
	LUA_FIELD(body, "body", userdata);
	cpBodyResetForces(body);
	return 0;
}


int cb_apply_force(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	double x = luaL_checknumber(L, 2);
	double y = luaL_checknumber(L, 3);
	lua_pop(L, 2);

	cpBody* body;
	LUA_FIELD(body, "body", userdata);
	cpBodyApplyForce(body, cpv(x*100, y*100), cpv(0, 0));
	return 0;
}



int cb_set_velocity(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	double x = luaL_checknumber(L, 2);
	double y = luaL_checknumber(L, 3);
	lua_pop(L, 2);

	cpBody* body;
	LUA_FIELD(body, "body", userdata);
	cpBodySetVel(body, cpv(x, y));
	return 0;
}


int cb_do_physics(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TTABLE);

	cpSpaceStep(space, 1.0/60.0);

	int n = luaL_len(L, 1);
	for(int i=1; i<=n; i++) {
		// get object
		lua_rawgeti(L, 1, i);

		// get object info
		cpBody* body;
		LUA_FIELD(body, "body", userdata);
		cpVect pos = cpBodyGetPos(body);

		// set new data
		LUA_SET_FIELD(pos.x, "x", number);
		LUA_SET_FIELD(pos.y, "y", number);
		LUA_SET_FIELD(cpBodyGetAngle(body), "angle", number);
	}

	lua_newtable(L);
	return 1;
}

void physics_finish()
{
	cpSpaceFree(space);
}
