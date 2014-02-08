#ifndef PHYSICS_H
#define PHYSICS_H

#include <lua.h>
#include <chipmunk.h>

extern cpSpace* space;

int physics_init(lua_State* L);
int physics_step(lua_State* L);
int physics_create_static_obj(lua_State* L);
int physics_finish(lua_State* L);

int physics_obj_pos(lua_State* L);

#endif


// vim: ts=4:sw=4:sts=4:expandtab
