#include "luah.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

#include <lualib.h>
#include <lauxlib.h>


extern int setenv (const char *, const char *, int);


lua_State* luah_init(lua_State* L)
{
	// initialize LUA
	setenv("LUA_PATH", "engine/?.lua;;", 0);
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


void luah_load_engine(lua_State* L)
{
	if(luaL_loadfile(L, "engine/newhope.lua") || lua_pcall(L, 0, 0, 0)) {
		luah_error(L, "can't load file: %s\n", lua_tostring(L, -1));
	}
}


void luah_start_engine(lua_State* L)
{
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

