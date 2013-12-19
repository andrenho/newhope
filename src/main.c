#include <lua.h>

#include "luah.h"

static void complement_engine_functions(lua_State* L);

int main()
{
	// initialize lua
	lua_State *L = luah_init(NULL);

	// load engine
	luah_load_engine(L);

	// complement engine with C functions
	complement_engine_functions(L);

	// start engine
	luah_start_engine(L);

	// clean up lua
	lua_close(L);

	return 0;
}


static void complement_engine_functions(lua_State* L)
{
}
