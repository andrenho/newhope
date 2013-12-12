#include "render.h"

int cb_render(lua_State* L)
{
	return 0;
}


int cb_window_tiles(lua_State* L)
{
	lua_newtable(L);
	return 1;
}
