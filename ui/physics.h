#ifndef PHYSICS_H
#define PHYSICS_H

#include "lua.h"

// world
void physics_init();
int cb_add_static_tile(lua_State* L);
int cb_step(lua_State* L);
void physics_finish();

// car
int cb_create_car_body(lua_State* L);
int cb_apply_force(lua_State* L);
int cb_reset_forces(lua_State* L);

// person
int cb_create_dynamic_person_body(lua_State* L);
int cb_create_static_person_body(lua_State* L);
int cb_set_dynamic_person_target(lua_State* L);

// player 
int cb_setup_player_collision_handler(lua_State* L);

#endif
