#include <lua.h>

#include "luah.h"
#include "ui.h"

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
	luah_end_engine(L);

	return 0;
}


static void complement_engine_functions(lua_State* L)
{
	// UI
	luah_set_c_function(L, "ui", "__c_init",    ui_c_init);
	luah_set_c_function(L, "ui", "now",         ui_now);
	luah_set_c_function(L, "ui", "wait",        ui_wait);
	luah_set_c_function(L, "ui", "__c_redraw",  ui_c_redraw);
	luah_set_c_function(L, "ui", "user_events", ui_user_events);
	luah_set_c_function(L, "ui", "clean_up",    ui_clean_up);
}
