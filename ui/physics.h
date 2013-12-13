#ifndef PHYSICS_H
#define PHYSICS_H

#include "lua.h"

void physics_init();
int cb_step(lua_State* L);
void physics_finish();

// person
int cb_create_person_body(lua_State* L);
int cb_set_person_target(lua_State* L);

//player 
int cb_setup_player_collision_handler(lua_State* L);

#endif
