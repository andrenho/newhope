#ifndef LUAH_H
#define LUAH_H

#include <lua.h>

lua_State* luah_init(lua_State* L);
void luah_load_engine(lua_State* L);
void luah_start_engine(lua_State* L);
void luah_error(lua_State* L, const char *fmt, ...);
void luah_stack_dump(lua_State* L);

#endif
