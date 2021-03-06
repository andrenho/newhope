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
	cpSpaceSetDamping(space, 0.2);
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
		if(body == 0)
			continue;
		cpVect pos = cpBodyGetPos(body);

		// set new data
		LUA_SET_FIELD(pos.x, "x", number);
		LUA_SET_FIELD(pos.y, "y", number);
		LUA_SET_FIELD(cpBodyGetAngle(body), "angle", number);
	}

	lua_newtable(L);
	return 1;
}

int cb_add_static_tile(lua_State* L)
{
	cpFloat x = luaL_checknumber(L, 2),
		y = luaL_checknumber(L, 3),
		w = luaL_checknumber(L, 4),
		h = luaL_checknumber(L, 5);
	cpShape *obj = cpBoxShapeNew2(space->staticBody, 
			cpBBNew(x-(w/2), y-(h/2), x+(w/2), y+(h/2)));
	cpShapeSetFriction(obj, 1);
	cpSpaceAddShape(space, obj);
	return 0;
}


void physics_finish()
{
	cpSpaceFree(space);
}


/***********
 *   CAR   *
 ***********/


int cb_create_car_body(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	
	// get car fields
	cpFloat x, y, angle;
	LUA_FIELD(x, "x", number);
	LUA_FIELD(y, "y", number);
	LUA_FIELD(angle, "angle", number);

	// get car attributes
	cpFloat w, h, mass;
	lua_pushstring(L, "attrib");
	lua_gettable(L, -2);
	LUA_FIELD(w, "w", number);
	LUA_FIELD(h, "h", number);
	LUA_FIELD(mass, "mass", number);
	lua_pop(L, 1);

	// create person body
	cpFloat moment = cpMomentForBox(mass, w, h);
	cpBody* body = cpSpaceAddBody(space, cpBodyNew(mass, moment));
	cpBodySetPos(body, cpv(x, y));
	cpBodySetAngle(body, angle);

	// create person shape
	cpShape* shape = cpSpaceAddShape(space, cpBoxShapeNew(body, w, h));
	cpShapeSetFriction(shape, 0);

	// save pointers to LUA
	LUA_SET_FIELD(body, "body", lightuserdata);
	LUA_SET_FIELD(shape, "shape", lightuserdata);
	
	return 0;
}


int cb_apply_force(lua_State* L)
{
	// get parameters: (1) car, (2) force, (3) relative_dir, (4) wheel
	luaL_checktype(L, 1, LUA_TTABLE);
	double force = luaL_checknumber(L, 2);
	double relative_dir = luaL_checknumber(L, 3);
	int wheel = luaL_checkinteger(L, 4);
	assert(wheel >= 0 || wheel <= 2);
	lua_pop(L, 3);

	// get object information
	cpBody* body;
	cpFloat angle;
	LUA_FIELD(body, "body", userdata);
	LUA_FIELD(angle, "angle", number);

	// get object attributes
	cpFloat wheels_radius;
	lua_pushstring(L, "attrib");
	lua_gettable(L, -2);
	LUA_FIELD(wheels_radius, "wheels_radius", number);

	// calculate vector relative direction
	cpVect rel_dir = cpv(cos(angle + relative_dir)*force, 
			sin(angle + relative_dir)*force);
	
	// calculate wheel position vector
	if(wheel == 0)
		wheels_radius = 0;
	else if(wheel == 2)
		wheels_radius -= wheels_radius;
	cpVect wheel_dir = cpv(cos(angle)*wheels_radius, sin(angle)*wheels_radius);

	// apply force
	cpBodyApplyForce(body, cpv(500, 0), cpvzero);

	return 0;
}

int cb_car_step(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	cpBody* body;
	LUA_FIELD(body, "body", userdata);

	// get lateral velocity
	cpVect normal = cpvperp(cpBodyGetRot(body));
	cpVect linear = cpvmult(normal, cpvdot(normal, cpBodyGetVel(body)));

	cpVect impulse = cpvmult(cpvneg(linear), cpBodyGetMass(body));
	printf("%f %f\n", impulse.x, impulse.y);
	cpBodyApplyImpulse(body, impulse, cpvzero);
}

int cb_car_set_angle(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	double n = luaL_checknumber(L, 2);
	lua_pop(L, 1);

	cpBody* body;
	LUA_FIELD(body, "body", userdata);
	
	cpBodySetAngle(body, cpBodyGetAngle(body)+n);
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


int cb_car_speed(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TTABLE);

	cpBody* body;
	LUA_FIELD(body, "body", userdata);

	lua_pushnumber(L, cpvlength(cpBodyGetForce(body))/ 800);
	return 1;
}

/**************
 *   PERSON   *
 **************/


int cb_create_static_person_body(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TTABLE);

	// get object fields
	cpFloat x, y;
	LUA_FIELD(x, "x", number);
	LUA_FIELD(y, "y", number);
	
	// create person body
	cpBody* body = cpSpaceAddBody(space, cpBodyNew(INFINITY, INFINITY));
	cpBodySetPos(body, cpv(x, y));

	// create person shape
	cpShape* shape = cpSpaceAddShape(space, 
			cpCircleShapeNew(body, 0.5, cpvzero));
	cpShapeSetFriction(shape, 0.1);

	// save pointers to LUA
	LUA_SET_FIELD(body, "body", lightuserdata);
	LUA_SET_FIELD(shape, "shape", lightuserdata);

	return 0;
}


int cb_create_dynamic_person_body(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TTABLE);

	// get object fields
	cpFloat x, y;
	LUA_FIELD(x, "x", number);
	LUA_FIELD(y, "y", number);
	
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
	LUA_SET_FIELD(body, "body", lightuserdata);
	LUA_SET_FIELD(shape, "shape", lightuserdata);
	LUA_SET_FIELD(target, "target", lightuserdata);
	LUA_SET_FIELD(joint, "joint", lightuserdata);
	
	return 0;
}


int cb_set_dynamic_person_target(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	double x = luaL_checknumber(L, 2);
	double y = luaL_checknumber(L, 3);
	lua_pop(L, 2);

	cpBody* target;
	LUA_FIELD(target, "target", userdata);
	if(target) {
		cpBodySetPos(target, cpv(x, y));
	}

	return 0;
}



/*************
 *   PLAYER  *
 *************/


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
	LUA_PUSH_MEMBER("object_bodies");
	lua_pushlightuserdata(L, a);
	lua_rawget(L, -2);
	if(lua_isnil(L, -1)) // hitted an object without a body
		return 1;
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
