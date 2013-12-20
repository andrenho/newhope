#ifndef STATICP_H
#define STATICP_H

#include <lua.h>

int staticp_init(lua_State* L);
int staticp_clean_up(lua_State* L);

#endif
