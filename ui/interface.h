#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdbool.h>

void if_load_libraries();
void if_start_game();
bool if_call(int narg, int nres);
void if_error(const char *fmt, ...);
void stack_dump();
void if_finish();

// macros
#define LUA_FIELD(c_field, field, type) { 	\
	lua_pushstring(L, field);	\
	lua_gettable(L, -2);		\
	(c_field) = lua_to ## type(L, -1); \
	lua_pop(L, 1); }
#define LUA_SET_FIELD(c_field, field, type) { \
	lua_pushstring(L, field); \
	lua_push ## type(L, c_field); \
	lua_settable(L, -3); }
#define LUA_PUSH_FUNCTION(f) { lua_pushstring(L, f); lua_gettable(L, -2); }
#define LUA_PUSH_METHOD(f) { LUA_PUSH_FUNCTION(f); lua_pushvalue(L, -2); }
#define LUA_PUSH_MEMBER(f) { LUA_PUSH_FUNCTION(f); }

#endif
