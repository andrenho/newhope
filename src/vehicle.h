#ifndef VEHICLE_H
#define VEHICLE_H

#include <chipmunk.h>
#include <lua.h>

typedef struct Vehicle {
	cpBody *vehicle_body, *rear_wheel_body, *front_wheel_body;
	cpShape *vehicle_shape, *rear_wheel_shape, *front_wheel_shape;
	cpConstraint *rear_wheel_joint1, *rear_wheel_joint2;
	cpConstraint *front_wheel_joint1, *front_wheel_joint2;
} Vehicle;

int vehicle_init(lua_State* L);
int vehicle_clean_up(lua_State* L);

#endif
