#include <lua.h>

#include "luah.h"
#include "ui.h"
#include "physics.h"
#include "staticp.h"
#include "dynamicp.h"
#include "vehicle.h"

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
	luah_set_c_function(L, "ui", "visible_tiles",  ui_visible_tiles);
	luah_set_c_function(L, "ui", "__message",      ui_message);
	luah_set_c_function(L, "ui", "clean_up",       ui_clean_up);

	// World
	luah_set_c_function(L, "World", "__init_physics", physics_init);
	luah_set_c_function(L, "World", "__physics_step", physics_step);
	luah_set_c_function(L, "World", "__physics_create_static_obj", physics_create_static_obj);
	luah_set_c_function(L, "World", "__finish_physics", physics_finish);

	// Object position
	luah_set_c_function(L, "Object", "__pos", physics_obj_pos);

	// Static person
	luah_set_c_function(L, "StaticPerson", "init_physics", staticp_init);
	luah_set_c_function(L, "StaticPerson", "clean_up", staticp_clean_up);

	// Dynamic person
	luah_set_c_function(L, "DynamicPerson", "init_physics", dynamicp_init);
	luah_set_c_function(L, "DynamicPerson", "set_target", dynamicp_set_target);
	luah_set_c_function(L, "DynamicPerson", "set_position", dynamicp_set_position);
	luah_set_c_function(L, "DynamicPerson", "clean_up", dynamicp_clean_up);

	// Vehicle
	luah_set_c_function(L, "Vehicle", "init_physics", vehicle_init);
	luah_set_c_function(L, "Vehicle", "__update_physics", vehicle_update);
	luah_set_c_function(L, "Vehicle", "clean_up", vehicle_clean_up);
}
