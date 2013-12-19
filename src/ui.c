#include "ui.h"

int ui_c_init(lua_State* L)
{
	lua_pushnil(L);
	return 1;
}


int ui_now(lua_State* L)
{
	lua_pushinteger(L, 0);
	return 1;
}


int ui_wait(lua_State* L)
{
	return 0;
}


int ui_c_redraw(lua_State* L)
{
	return 0;
}


int ui_user_events(lua_State* L)
{
	lua_newtable(L);
	return 1;
}
