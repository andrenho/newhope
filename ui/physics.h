#ifndef PHYSICS_H
#define PHYSICS_H

#include "lua.h"

void physics_init();
int cb_add_static_object(lua_State* L);
int cb_add_dynamic_object(lua_State* L);
int cb_step(lua_State* L);
int cb_apply_force(lua_State* L);
int cb_reset_forces(lua_State* L);
int cb_set_velocity(lua_State* L);
int cb_setup_player_collision_handler(lua_State* L);
void physics_finish();

#endif
