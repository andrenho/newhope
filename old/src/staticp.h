#ifndef STATICP_H
#define STATICP_H

#include <lua.h>

int staticp_init(lua_State* L);
int staticp_clean_up(lua_State* L);

#endif


// vim: ts=4:sw=4:sts=4:expandtab
