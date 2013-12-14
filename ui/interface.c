#include "interface.h"

#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "physics.h"
#include "render.h"
#include "ui.h"

// global variables
lua_State *L = NULL;
bool if_reload_engine;

// function prototypes
extern int setenv (const char *, const char *, int);
static int traceback(lua_State *L);

// callbacks
struct Callback {
	const char* name;
	lua_CFunction fct;
} callbacks[] = {

	// callbacks
	{ "active", 		 cb_active },
	{ "current_time_ms",	 cb_current_time_ms },
	{ "finish_ui",		 cb_finish_ui },
	{ "get_commands",	 cb_get_commands },
	{ "initialize_ui", 	 cb_initialize_ui },
	{ "sleep_ms",		 cb_sleep_ms },
	{ "render",		 cb_render_physics },
	{ "window_tiles",	 cb_window_tiles },

	// physics
	{ "step",		 		cb_step },
	{ "add_static_tile",                    cb_add_static_tile },
	{ "create_dynamic_person_body", 	cb_create_dynamic_person_body },
	{ "create_static_person_body", 		cb_create_static_person_body },
	{ "create_car_body",                    cb_create_car_body },
	{ "set_dynamic_person_target", 		cb_set_dynamic_person_target },
	{ "setup_player_collision_handler", 	cb_setup_player_collision_handler },
	
	{ NULL, NULL }
};

void if_load_libraries()
{
	if_reload_engine = false;

	// initialize LUA
	setenv("LUA_PATH_5_2", "engine/?.lua;engine/?/init.lua;;", 0);
	if(!L) {
		// first run
		L = luaL_newstate();
		luaL_openlibs(L);
	} else {
		// engine was reloaded, clear stack
		lua_settop(L, 0);
	}

	// load libraries and classes
	if(luaL_loadfile(L, "engine/libs.lua") || lua_pcall(L, 0, 0, 0)) {
		if_error("can't load file: %s\n", lua_tostring(L, -1));
	}

	// avoid warnings
	(void) stack_dump;
}

void if_start_game()
{
	int i = 0;

	// game.Game:new
	lua_getglobal(L, "game");
	lua_pushstring(L, "Game");
	lua_gettable(L, -2);
	lua_remove(L, 1);
	LUA_PUSH_METHOD("new");

	// parameter 1 - world
	lua_getglobal(L, "World");
	LUA_PUSH_METHOD("new");
	if_call(1, 1);
	lua_remove(L, 4);

	// parameter 2 - callbacks
	lua_newtable(L);
	while(callbacks[i].name) {
		LUA_SET_FIELD(callbacks[i].fct, callbacks[i].name, cfunction);
		++i;
	}

	// initialize game
	if_call(3, 1);
	lua_remove(L, 1);
	
	// start game
	LUA_PUSH_METHOD("start");
	if_call(1, 0);
}

void if_error(const char *fmt, ...) 
{
	lua_settop(L, 0);

	va_list argp;
	char s[512];

	va_start(argp, fmt);
	vsnprintf(s, 511, fmt, argp);
	fprintf(stderr, "%s\n", s);
	va_end(argp);
	exit(EXIT_FAILURE);
	/* callbacks.lua_error(s);
	stack_dump();
	if_reload_engine = true; */
}

bool if_call(int narg, int nres)
{
	int base = lua_gettop(L) - narg;
	lua_pushcfunction(L, traceback);
	lua_insert(L, base);
	if(if_reload_engine)
		return false;
	if(lua_pcall(L, (narg), (nres), base) != LUA_OK) { 
		if_error("%s\n", lua_tostring(L, -1)); 
		lua_remove(L, base);
		return false; 
	} 
	lua_remove(L, base);
	return true;
}

void stack_dump() 
{
	int i;
	int top = lua_gettop(L); /* depth of the stack */
	for (i = 1; i <= top; i++) { /* repeat for each level */
		int t = lua_type(L, i);
		printf("(%d/%d)", i, i-lua_gettop(L)-1);
		switch (t) {
		case LUA_TSTRING:
			printf("'%s'", lua_tostring(L, i));
			break;
		case LUA_TBOOLEAN:
			printf(lua_toboolean(L, i) ? "true" : "false");
			break;
		case LUA_TNUMBER:
			printf("%g", lua_tonumber(L, i));
			break;
		case LUA_TTABLE:
			lua_getglobal(L, "tostring");
			lua_pushvalue(L, i);
			if(lua_pcall(L, 1, 1, 0) == LUA_OK)
				printf("%s", lua_tolstring(L, -1, NULL));
			else
				printf("table: ?");
			lua_pop(L, 1);
			break;
		default: 
			printf("%s", lua_typename(L, t));
			break;
		}
		printf(" "); /* put a separator */
	}
	printf("\n"); /* end the listing */
}

void if_finish()
{
	lua_close(L);
}

/********************
 * STATIC FUNCTIONS *
 ********************/

static int traceback(lua_State *L)
{
	const char *msg = lua_tostring(L, 1);
	if(msg) {
		luaL_traceback(L, L, msg, 1);
	} else if(!lua_isnoneornil(L, 1)) {
		if(!luaL_callmeta(L, 1, "__tostring")) {
			lua_pushliteral(L, "(no error message)");
		}
	}
	return 1;
}
