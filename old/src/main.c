#include <lua.h>

#include "luah.h"
#include "ui.h"
#include "physics.h"
#include "staticp.h"
#include "dynamicp.h"
#include "vehicle.h"
#include "dialog.h"

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
	luah_set_c_function(L, "ui", "__c_init",       ui_c_init);
	luah_set_c_function(L, "ui", "now",            ui_now);
	luah_set_c_function(L, "ui", "wait",           ui_wait);
	luah_set_c_function(L, "ui", "render",         ui_render);
	luah_set_c_function(L, "ui", "user_events",    ui_user_events);
	luah_set_c_function(L, "ui", "keyboard_state", ui_keyboard_state);
	luah_set_c_function(L, "ui", "center_screen",  ui_center_screen);
	luah_set_c_function(L, "ui", "visible_tiles",  ui_visible_tiles);
	luah_set_c_function(L, "ui", "show_minimap",   ui_show_minimap);
	luah_set_c_function(L, "ui", "clean_up",       ui_clean_up);

	// UI dialog
	luah_set_c_function(L, "ui", "__message",      dialog_message);
	luah_set_c_function(L, "ui", "buy_sell",       dialog_buysell);

	// World
	luah_set_c_function(L, "World", "__init_physics", physics_init);
	luah_set_c_function(L, "World", "__physics_step", physics_step);
	luah_set_c_function(L, "World", "__physics_create_static_obj", physics_create_static_obj);
	luah_set_c_function(L, "World", "__finish_physics", physics_finish);

	// Object position
	luah_set_c_function(L, "Object", "__pos", physics_obj_pos);
	luah_set_c_function(L, "Object", "init_physics", staticp_init);
	luah_set_c_function(L, "Object", "clean_up", staticp_clean_up);

	// Dynamic person
	luah_set_c_function(L, "Person", "init_physics", dynamicp_init);
	luah_set_c_function(L, "Person", "set_target", dynamicp_set_target);
	luah_set_c_function(L, "Person", "set_position", dynamicp_set_position);
	luah_set_c_function(L, "Person", "clean_up", dynamicp_clean_up);

	// Vehicle
	luah_set_c_function(L, "Vehicle", "init_physics", vehicle_init);
	luah_set_c_function(L, "Vehicle", "__update_physics", vehicle_update);
	luah_set_c_function(L, "Vehicle", "clean_up", vehicle_clean_up);
}
