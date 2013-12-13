#include "physics.h"

#include <assert.h>
#include <chipmunk.h>
#include "lua.h"
#include "lauxlib.h"

#include "interface.h"

extern lua_State *L;

static cpSpace* space;
static cpBody* player_body = NULL;

void physics_init()
{
	// initialize physics
	cpVect gravity = cpv(0, 0);
	space = cpSpaceNew();
	cpSpaceSetGravity(space, gravity);
}

int cb_add_dynamic_object(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TTABLE);

	cpFloat x, y, w, h, angle, mass, vel_limit;
	int model;

	// get object fields
	LUA_FIELD(x, "x", number);
	LUA_FIELD(y, "y", number);
	LUA_FIELD(w, "w", number);
	LUA_FIELD(h, "h", number);
	LUA_FIELD(angle, "angle", number);
	LUA_FIELD(mass, "mass", number);
	if(mass == -1) {
		mass = INFINITY;
	}
	LUA_FIELD(vel_limit, "velocity_limit", number);
	LUA_FIELD(model, "physics_model", integer);

	(void) angle, (void) mass;

	// create chipmunk object
	cpBody* body;
	cpShape* shape;
	if(model == 1 || model == 2) {
		cpFloat moment = cpMomentForCircle(mass, 0, w, cpv(0, 0));
		body = cpSpaceAddBody(space, cpBodyNew(mass, moment));
		cpBodySetPos(body, cpv(x, y));
		cpBodySetAngle(body, 0);
		shape = cpSpaceAddShape(space, cpCircleShapeNew(body, w, cpv(0, 0)));
		shape->collision_type = 0;
	}
	else {
		abort();
	}

	// maximum velocity
	if(vel_limit) {
		cpBodySetVelLimit(body, vel_limit);
	}

	// save pointers
	LUA_SET_FIELD(body, "body", lightuserdata);
	LUA_SET_FIELD(shape, "shape", lightuserdata);

	return 0;
}


static int handle_player_collision(cpArbiter* arb, cpSpace *sp, void *data)
{
	cpBody *a, *b;
	cpArbiterGetBodies(arb, &a, &b);

	// exclude player
	if(a == player_body) {
		a = b;
	}

	// push player:collision
	LUA_PUSH_PLAYER();
	LUA_PUSH_METHOD("collision");

	// find 'against'
	LUA_PUSH_WORLD();
	LUA_PUSH_MEMBER("dynamic_object_bodies");
	lua_pushlightuserdata(L, a);
	lua_rawget(L, -2);
	lua_remove(L, -2);
	lua_remove(L, -2);
	
	// call player:collision(against)
	if_call(2, 0);

	return 1;
}


int cb_setup_player_collision_handler(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TTABLE);

	cpBody* body;
	LUA_FIELD(body, "body", userdata);
	cpSpaceAddCollisionHandler(space, 0, 0, handle_player_collision, NULL, 
			NULL, NULL, NULL);
	player_body = body;
	
	return 1;
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


int cb_step(lua_State* L)
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

		// get object type
		int model;
		LUA_FIELD(model, "physics_model", integer);
		assert(model != 0);
		/*
		if(model == 1 || model == 2) {
			cpBodySetAngle(body, 0);
			// TODO - maybe model 2 can be a static object ?
			if(model == 2) {
				double orig_x, orig_y;
				LUA_SET_FIELD(orig_x, "original_x", number);
				LUA_SET_FIELD(orig_y, "original)y", number);
				cpBodySetPos(body, cpv(orig_x, orig_y));
			}
		}*/

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
