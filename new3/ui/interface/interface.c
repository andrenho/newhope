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

// preprocessor
#define LUA_FIELD(c_field, field, type) { 	\
	lua_pushstring(L, #field);	\
	lua_gettable(L, -2);		\
	(c_field) = lua_to ## type(L, -1); \
	lua_pop(L, 1); }

#define LUA_PUSH_HERO() { lua_pushstring(L, "hero"); lua_gettable(L, -2); }
#define LUA_PUSH_FUNCTION(f) { lua_pushstring(L, f); lua_gettable(L, -2); }
#define LUA_PUSH_METHOD(f) { LUA_PUSH_FUNCTION(f); lua_pushvalue(L, -2); }
#define LUA_CALL(narg, nres) { if(lua_pcall(L, (narg), (nres), 0) != LUA_OK) { error("%s\n", lua_tostring(L, -1)); } }

// function prototypes
extern int setenv (const char *, const char *, int);
static void check_stack();
static void error(const char *fmt, ...);
static void stack_dump();
static void if_person_on_stack(Person* person);

void if_init()
{
	setenv("LUA_PATH_5_2", "../../engine/?.lua;;", 0);
	L = luaL_newstate();
	luaL_openlibs(L);
	if(luaL_loadfile(L, "../../engine/world.lua") || lua_pcall(L, 0, 1, 0)) {
		error("can't load file: %s\n", lua_tostring(L, -1));
	}

	(void) stack_dump;
}


void if_finish()
{
	lua_close(L);
}


void if_next_frame()
{
	check_stack();

	LUA_PUSH_METHOD("step");
	LUA_CALL(1, 0);

	check_stack();
}


void if_hero_move(int speed, int direction)
{
	check_stack();

	LUA_PUSH_HERO();

	// set speed
	if(speed != 0) {
		LUA_PUSH_METHOD("turn_to");
		lua_pushinteger(L, direction);
		LUA_CALL(2, 0);
	}

	// set direction
	LUA_PUSH_METHOD("change_speed");
	lua_pushinteger(L, speed);
	LUA_CALL(2, 0);

	lua_pop(L, 1);

	check_stack();
}


void if_hero_position(double* x, double* y)
{
	check_stack();

	LUA_PUSH_HERO();

	// get x
	LUA_FIELD(*x, x, number);
	LUA_FIELD(*y, y, number);

	lua_pop(L, 1);

	check_stack();
}


uint8_t if_world_tile_stack(int x, int y, BLOCK stack[10])
{
	check_stack();

	// call function
	LUA_PUSH_METHOD("tile_stack");
	lua_pushinteger(L, x);
	lua_pushinteger(L, y);
	LUA_CALL(3, 1);

	// get response
	int n = luaL_len(L, -1);
	for(int i=0; i<n; i++) {
		lua_rawgeti(L, -1, i+1);
		stack[i] = lua_tonumber(L, -1);
		lua_pop(L, 1);
	}
	lua_pop(L, 1);

	check_stack();
	return n;
}


int if_people_visible(int x1, int y1, int x2, int y2, Person** people)
{
	check_stack();

	// call function
	LUA_PUSH_METHOD("people_in_area");
	lua_pushinteger(L, x1);
	lua_pushinteger(L, y1);
	lua_pushinteger(L, x2);
	lua_pushinteger(L, y2);
	LUA_CALL(5, 1);

	// get response
	int n = luaL_len(L, -1);
	*people = calloc(n, sizeof(Person));
	for(int i=0; i<n; i++) {
		lua_rawgeti(L, -1, i+1);
		if_person_on_stack(&*people[i]);
		lua_pop(L, 1);
	}
	lua_pop(L, 1);

	check_stack();
	return n;
}


/*
 *
 * STATIC FUNCTIONS
 *
 */

// return data of the person who is in the top of the stack
static void if_person_on_stack(Person* person)
{
	LUA_FIELD(person->x, x,      number);
	LUA_FIELD(person->y, y,      number);
	LUA_FIELD(person->image,     image, integer);
	LUA_FIELD(person->direction, direction, integer);
}


static void check_stack()
{
	assert(lua_istable(L, -1));
	assert(lua_gettop(L) == 1);
}

static void error(const char *fmt, ...) 
{
	va_list argp;
	va_start(argp, fmt);
	vfprintf(stderr, fmt, argp);
	va_end(argp);
	lua_close(L);
	exit(EXIT_FAILURE);
}

static void stack_dump() 
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

