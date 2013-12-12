#include "ui.h"

int cb_initialize_ui(lua_State* L)
{
	return 0;
}


int cb_active(lua_State* L)
{
	lua_pushboolean(L, 0);
	return 1;
}


int cb_finish_ui(lua_State* L)
{
	return 0;
}


int cb_current_time_us(lua_State* L)
{
	lua_pushnumber(L, 0);
	return 1;
}


int cb_get_commands(lua_State* L)
{
	lua_newtable(L);
	return 1;
}


int cb_sleep_us(lua_State* L)
{
	return 0;
}
