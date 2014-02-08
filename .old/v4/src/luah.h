#ifndef LUAH_H
#define LUAH_H

#include <stdbool.h>

#include <lua.h>

lua_State* luah_init(lua_State* L);
void luah_load_engine(lua_State* L);
bool luah_call(lua_State* L, int narg, int nres);
void luah_set_c_function(lua_State* L, const char* lua_object, 
        const char* lua_func_name, lua_CFunction c_func);

void luah_start_engine(lua_State* L);
void luah_end_engine(lua_State* L);

void luah_error(lua_State* L, const char *fmt, ...);
void luah_stack_dump(lua_State* L);

//
// macros
//
#define LUA_FIELD(L, c_field, field, type) { 	\
    lua_pushstring(L, field);	\
    lua_gettable(L, -2);		\
    (c_field) = lua_to ## type(L, -1); \
    lua_pop(L, 1); }

#define LUA_SET_FIELD(L, c_field, field, type) { \
    lua_pushstring(L, field); \
    lua_push ## type(L, c_field); \
    lua_settable(L, -3); }

#define LUA_PUSH_WORLD(L) { lua_getglobal(L, "world"); }
#define LUA_PUSH_PLAYER(L) { LUA_PUSH_WORLD(L); lua_pushstring(L, "player"); lua_gettable(L, -2); lua_remove(L, -2); }
#define LUA_PUSH_FUNCTION(L, f) { lua_pushstring(L, f); lua_gettable(L, -2); }
#define LUA_PUSH_METHOD(L, f) { LUA_PUSH_FUNCTION(L, f); lua_pushvalue(L, -2); }
#define LUA_PUSH_MEMBER(L, f) { LUA_PUSH_FUNCTION(L, f); }


#endif


// vim: ts=4:sw=4:sts=4:expandtab
