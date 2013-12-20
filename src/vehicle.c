#include "vehicle.h"

#include <lauxlib.h>

#include "luah.h"

extern cpSpace* space;

Vehicle* create_vehicle(cpFloat x, cpFloat y, cpFloat angle, cpFloat w, cpFloat h)
{
	Vehicle* v = malloc(sizeof(Vehicle));

	// create vehicle
	cpFloat mass = 150;
	cpFloat moment = cpMomentForBox(mass, w, h);
	v->vehicle_body = cpSpaceAddBody(space, cpBodyNew(mass, moment));
	v->vehicle_shape = cpSpaceAddShape(space, cpBoxShapeNew2(v->vehicle_body, 
				cpBBNew(-(w/2), -(h/2), (w/2), (h/2))));
	cpBodySetPos(v->vehicle_body, cpv(x,y));
	cpShapeSetGroup(v->vehicle_shape, 1);

	// create wheels
	cpFloat ww = 0.5, wh = 1.25;
	cpFloat wheel_mass = 50;
	cpFloat wheel_moment = cpMomentForBox(wheel_mass, ww, wh);

	// create rear wheel
	v->rear_wheel_body = cpSpaceAddBody(space, cpBodyNew(mass, wheel_moment));
	v->rear_wheel_shape = cpSpaceAddShape(space, 
			cpBoxShapeNew2(v->rear_wheel_body,
				cpBBNew(-(ww/2), -(wh/2), (ww/2), (wh/2))));
	cpShapeSetGroup(v->rear_wheel_shape, 2);
	cpBodySetPos(v->rear_wheel_body, cpv(x, y-(h/2)+1.5));
	v->rear_wheel_joint1 = cpSpaceAddConstraint(space, 
			cpPivotJointNew(v->vehicle_body, v->rear_wheel_body, 
				cpBodyGetPos(v->rear_wheel_body)));
	v->rear_wheel_joint2 = cpSpaceAddConstraint(space, 
			cpRotaryLimitJointNew(v->vehicle_body, v->rear_wheel_body, 
				0, 0));

	// create front wheel
	v->front_wheel_body = cpSpaceAddBody(space, cpBodyNew(mass, wheel_moment));
	v->front_wheel_shape = cpSpaceAddShape(space, 
			cpBoxShapeNew2(v->front_wheel_body,
				cpBBNew(-(ww/2), -(wh/2), (ww/2), (wh/2))));
	cpShapeSetGroup(v->front_wheel_shape, 2);
	cpBodySetPos(v->front_wheel_body, cpv(x, y+(h/2)-1.5));
	v->front_wheel_joint1 = cpSpaceAddConstraint(space, 
			cpPivotJointNew(v->vehicle_body, v->front_wheel_body, 
				cpBodyGetPos(v->front_wheel_body)));
	v->front_wheel_joint2 = cpSpaceAddConstraint(space, 
			cpRotaryLimitJointNew(v->vehicle_body, v->front_wheel_body, 
				-M_PI/6, M_PI/6));

	return v;
}

int vehicle_init(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	
	// get car fields
	cpFloat x, y, angle;
	LUA_FIELD(L, x, "x", number);
	LUA_FIELD(L, y, "y", number);
	LUA_FIELD(L, angle, "angle", number);

	// get car attributes
	cpFloat w, h;
	lua_pushstring(L, "attrib");
	lua_gettable(L, -2);
	LUA_FIELD(L, w, "w", number);
	LUA_FIELD(L, h, "h", number);
	lua_pop(L, 1);

	// create vehicle
	Vehicle* vehicle = create_vehicle(x, y, angle, w, h);

	// save pointers to LUA
	LUA_SET_FIELD(L, vehicle, "c_ptr", lightuserdata);
	
	return 0;
}


int vehicle_clean_up(lua_State* L)
{
	Vehicle* v;
	LUA_FIELD(L, v, "c_ptr", userdata);

	cpSpaceRemoveShape(space, v->vehicle_shape);
	cpSpaceRemoveShape(space, v->rear_wheel_shape);
	cpSpaceRemoveShape(space, v->front_wheel_shape);
	cpSpaceRemoveConstraint(space, v->rear_wheel_joint1);
	cpSpaceRemoveConstraint(space, v->rear_wheel_joint2);
	cpSpaceRemoveConstraint(space, v->front_wheel_joint1);
	cpSpaceRemoveConstraint(space, v->front_wheel_joint2);
	cpSpaceRemoveBody(space, v->vehicle_body);
	cpSpaceRemoveBody(space, v->rear_wheel_body);
	cpSpaceRemoveBody(space, v->front_wheel_body);

	cpShapeFree(v->vehicle_shape);
	cpShapeFree(v->rear_wheel_shape);
	cpShapeFree(v->front_wheel_shape);
	cpConstraintFree(v->rear_wheel_joint1);
	cpConstraintFree(v->rear_wheel_joint2);
	cpConstraintFree(v->front_wheel_joint1);
	cpConstraintFree(v->front_wheel_joint2);
	cpBodyFree(v->vehicle_body);
	cpBodyFree(v->rear_wheel_body);
	cpBodyFree(v->front_wheel_body);

	free(v);

	return 0;
}
