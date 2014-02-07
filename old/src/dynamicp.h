#ifndef DYNAMICP_H
#define DYNAMICP_H

#include <lua.h>

int dynamicp_init(lua_State* L);
int dynamicp_set_target(lua_State* L);
int dynamicp_set_position(lua_State* L);
int dynamicp_clean_up(lua_State* L);

#endif


// vim: ts=4:sw=4:sts=4:expandtab
