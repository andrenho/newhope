#include "interface.h"

#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

// IMPORTANT: always leave a copy of 'world' in the stack

// global variables
static lua_State *L = NULL;

// function prototypes
extern int setenv (const char *, const char *, int);
static void error(lua_State *L, const char *fmt, ...);
static void stack_dump(lua_State *L); 

void if_init()
{
	setenv("LUA_PATH_5_2", "../../engine/?.lua;;", 0);
	L = luaL_newstate();
	luaL_openlibs(L);
	if(luaL_loadfile(L, "../../engine/world.lua") || lua_pcall(L, 0, 1, 0)) {
		error(L, "can't load file: %s", lua_tostring(L, -1));
	}

	(void) stack_dump;
}


void if_finish()
{
	lua_close(L);
}


void if_next_frame()
{
}


void if_hero_position(double* x, double* y)
{
	x = y = 0;
}


uint8_t if_world_tile_stack(int x, int y, BLOCK stack[10])
{
	assert(lua_istable(L, -1));

	// get function
	lua_pushstring(L, "tile_stack");
	lua_gettable(L, -2);

	// push parameters
	lua_pushvalue(L, 1); // self (world)
	lua_pushinteger(L, x);
	lua_pushinteger(L, y);

	// call function
	if(lua_pcall(L, 3, 1, 0) != LUA_OK) {
		error(L, "error running 'world.tile_stack': %s", 
				lua_tostring(L, -1));
	}

	// fill stack
	int n = luaL_len(L, -1);
	for(int i=0; i<n; i++) {
		lua_rawgeti(L, -1, i+1);
		stack[i] = lua_tonumber(L, -1);
		lua_pop(L, 1);
	}
	lua_pop(L, 1);

	assert(lua_istable(L, -1));
	return n;
}


/*
 *
 * STATIC FUNCTIONS
 *
 */

static void error(lua_State *L, const char *fmt, ...) 
{
	va_list argp;
	va_start(argp, fmt);
	vfprintf(stderr, fmt, argp);
	va_end(argp);
	lua_close(L);
	exit(EXIT_FAILURE);
}

static void stack_dump(lua_State *L) 
{
	int i;
	int top = lua_gettop(L); /* depth of the stack */
	for (i = 1; i <= top; i++) { /* repeat for each level */
		int t = lua_type(L, i);
		switch (t) {
		case LUA_TSTRING: { /* strings */
					  printf("'%s'", lua_tostring(L, i));
					  break;
				  }
		case LUA_TBOOLEAN: { /* booleans */
					   printf(lua_toboolean(L, i) ? "true" : "false");
					   break;
				   }
		case LUA_TNUMBER: { /* numbers */
					  printf("%g", lua_tonumber(L, i));
					  break;
				  }
		default: { /* other values */
				 printf("%s", lua_typename(L, t));
				 break;
			 }
		}
		printf(" "); /* put a separator */
	}
	printf("\n"); /* end the listing */
}

