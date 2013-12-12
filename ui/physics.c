#include "physics.h"

void physics_init()
{
}

int cb_do_physics(lua_State* L)
{
	lua_newtable(L);
	return 1;
}

void physics_finish()
{
}
