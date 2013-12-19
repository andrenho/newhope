#include "luah.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <lualib.h>
#include <lauxlib.h>


// function prototypes
extern int setenv (const char *, const char *, int);
static int traceback(lua_State *L);


lua_State* luah_init(lua_State* L)
{
	// initialize LUA
	setenv("LUA_PATH", "engine/?.lua;engine/?/init.lua;;", 0);
	if(!L) {
		// first run
		L = luaL_newstate();
		luaL_openlibs(L);
	} else {
		// engine was reloaded, clear stack
		lua_settop(L, 0);
	}

	return L;
}


bool luah_call(lua_State* L, int narg, int nres)
{
	int base = lua_gettop(L) - narg;
	lua_pushcfunction(L, traceback);
	lua_insert(L, base);
	if(lua_pcall(L, (narg), (nres), base) != LUA_OK) { 
		luah_error(L, "%s\n", lua_tostring(L, -1)); 
		lua_remove(L, base);
		return false; 
	} 
	lua_remove(L, base);
	return true;
}


void luah_load_engine(lua_State* L)
{
	if(luaL_loadfile(L, "engine/newhope.lua") || lua_pcall(L, 0, 0, 0)) {
		luah_error(L, "can't load file: %s\n", lua_tostring(L, -1));
	}
}


void luah_start_engine(lua_State* L)
{
	lua_getglobal(L, "game");
	LUA_PUSH_METHOD(L, "start");
	luah_call(L, 1, 0);
}


void luah_set_c_function(lua_State* L, const char* lua_object, 
		const char* lua_func_name, lua_CFunction c_func)
{
	char s[512];
	snprintf(s, 511, "return %s", lua_object);
	luaL_loadstring(L, s);
	if(lua_pcall(L, 0, LUA_MULTRET, 0)) {
		luah_error(L, "can't find function: %s\n", lua_tostring(L, -1));
	}
	assert(lua_type(L, -1) == LUA_TTABLE);
	LUA_SET_FIELD(L, c_func, lua_func_name, cfunction);
}


void luah_error(lua_State* L, const char *fmt, ...) 
{
	lua_settop(L, 0);

	va_list argp;
	char s[512];

	va_start(argp, fmt);
	vsnprintf(s, 511, fmt, argp);
	fprintf(stderr, "%s", s);
	va_end(argp);
	luah_stack_dump(L);
	exit(1); // TODO
}


void luah_stack_dump(lua_State* L)
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
			assert(lua_pcall(L, 1, 1, 0) == LUA_OK);
			printf("%s", lua_tolstring(L, -1, NULL));
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


/**********************
 *  STATIC FUNCTIONS  *
 **********************/

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
