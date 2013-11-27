#include "interface.h"

#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

// global variables
lua_State *L = NULL;

// function prototypes
extern int setenv (const char *, const char *, int);
static void stack_dump();
static void if_person_on_stack(Person* person);
static void if_car_on_stack(Car* car);
extern CALLBACK callbacks;

bool if_reload_engine;

/*
 * INITIALIZATION
 */


void if_init()
{
	if_reload_engine = false;

	setenv("LUA_PATH_5_2", "../../engine/?.lua;;", 0);
	
	// initialize LUA
	if(!L) {
		// first run
		L = luaL_newstate();
		luaL_openlibs(L);
	} else {
		// engine was reloaded, clear stack
		lua_settop(L, 0);
	}

	// load classes
	if(luaL_loadfile(L, "../../engine/newhope.lua") || lua_pcall(L, 0, 0, 0)) {
		if_error("can't load file: %s\n", lua_tostring(L, -1));
	}

	(void) stack_dump;
}


void if_init_world(int w, int h)
{
	// initialize world
	lua_getglobal(L, "World");
	LUA_PUSH_METHOD("new");
	lua_pushinteger(L, w);
	lua_pushinteger(L, h);
	if(!if_call(3, 1))
		return;
	lua_setglobal(L, "world");
	lua_pop(L, 1);
}

void if_finish()
{
	lua_close(L);
}


/*
 * REQUESTS
 */


void if_next_frame()
{
	check_stack();

	LUA_PUSH_WORLD();
	LUA_PUSH_METHOD("step");
	if(!if_call(1, 0))
		return;
	lua_pop(L, 1);

	check_stack();
}


void if_player_move(int speed, double direction)
{
	check_stack();

	LUA_PUSH_PLAYER();

	// set speed
	if(speed != 0) {
		LUA_PUSH_METHOD("turn_to");
		lua_pushnumber(L, direction);
		if(!if_call(2, 0))
		return;
	}

	// set direction
	LUA_PUSH_METHOD("change_speed");
	lua_pushinteger(L, speed);
	if(!if_call(2, 0))
		return;

	lua_pop(L, 1);

	check_stack();
}


/*
 * QUERIES
 */


void if_player_position(double* x, double* y)
{
	check_stack();

	LUA_PUSH_PLAYER();

	// get x
	LUA_FIELD(*x, "x", number);
	LUA_FIELD(*y, "y", number);

	lua_pop(L, 1);

	check_stack();
}


uint8_t if_world_tiles(int x, int y, BLOCK stack[10])
{
	check_stack();

	// call function
	LUA_PUSH_WORLD();
	LUA_PUSH_METHOD("tiles");
	lua_pushinteger(L, x);
	lua_pushinteger(L, y);
	if(!if_call(3, 1))
		return 0;

	// get response
	int n = luaL_len(L, -1);
	for(int i=0; i<n; i++) {
		lua_rawgeti(L, -1, i+1);
		stack[i] = lua_tonumber(L, -1);
		lua_pop(L, 1);
	}
	lua_pop(L, 2);

	check_stack();
	return n;
}


int if_people_visible(int x1, int y1, int x2, int y2, Person** people)
{
	check_stack();

	// call function
	LUA_PUSH_WORLD();
	LUA_PUSH_METHOD("people_in_area");
	lua_pushinteger(L, x1);
	lua_pushinteger(L, y1);
	lua_pushinteger(L, x2);
	lua_pushinteger(L, y2);
	if(!if_call(5, 1))
		return 0;

	// get response
	int n = luaL_len(L, -1);
	*people = calloc(n, sizeof(Person));
	for(int i=0; i<n; i++) {
		lua_rawgeti(L, -1, i+1);
		if_person_on_stack(&(*people)[i]);
		lua_pop(L, 1);
	}
	lua_pop(L, 2);

	check_stack();
	return n;
}


int if_cars_visible(int x1, int y1, int x2, int y2, Car** cars)
{
	check_stack();

	// call function
	LUA_PUSH_WORLD();
	LUA_PUSH_METHOD("cars_in_area");
	lua_pushinteger(L, x1);
	lua_pushinteger(L, y1);
	lua_pushinteger(L, x2);
	lua_pushinteger(L, y2);
	if(!if_call(5, 1))
		return 0;

	// get response
	int n = luaL_len(L, -1);
	*cars = calloc(n, sizeof(Car));
	for(int i=0; i<n; i++) {
		lua_rawgeti(L, -1, i+1);
		if_car_on_stack(&(*cars)[i]);
		lua_pop(L, 1);
	}
	lua_pop(L, 2);

	check_stack();
	return n;
}


int if_wrap(char* str, int columns, char*** ret)
{
	check_stack();

	// call function
	lua_getglobal(L, "string");
	LUA_PUSH_FUNCTION("wrap");
	lua_pushstring(L, str);
	lua_pushinteger(L, columns);
	if(!if_call(2, 1))
		return 0;

	// get results
	int n = luaL_len(L, -1);
	*ret = calloc(sizeof(char*), n);
	for(int i=0; i<n; i++) {
		lua_rawgeti(L, -1, i+1);
		(*ret)[i] = strdup(lua_tostring(L, -1));
		lua_pop(L, 1);
	}

	lua_pop(L, 2);
	check_stack();

	return n;
}


void check_stack()
{
	if(if_reload_engine)
		return;
	if(lua_gettop(L) != 0) {
		stack_dump();
		abort();
	}
}


void if_error(const char *fmt, ...) 
{
	lua_settop(L, 0);

	va_list argp;
	char s[512];

	va_start(argp, fmt);
	vsnprintf(s, 511, fmt, argp);
	fprintf(stderr, "%s", s);
	va_end(argp);
	callbacks.lua_error(s);
	stack_dump();
	if_reload_engine = true;
}

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


/*
 *
 * STATIC FUNCTIONS
 *
 */

// return data of the person who is in the top of the stack
static void if_person_on_stack(Person* person)
{
	LUA_FIELD(person->x, "x",      number);
	LUA_FIELD(person->y, "y",      number);
	LUA_FIELD(person->image,     "image", integer);
	LUA_FIELD(person->direction, "direction", number);
}


// return data of the car who is in the top of the stack
static void if_car_on_stack(Car* car)
{
	LUA_FIELD(car->x, "x", number);
	LUA_FIELD(car->y, "y", number);
	LUA_FIELD(car->direction, "direction", number);

	// get model
	lua_pushstring(L, "model");
	lua_gettable(L, -2);
	LUA_FIELD(car->model, "id", integer);
	LUA_FIELD(car->w, "w", number);
	LUA_FIELD(car->h, "h", number);
	lua_pop(L, 1);
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

