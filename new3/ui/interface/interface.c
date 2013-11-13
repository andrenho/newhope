#include "interface.h"

#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

// IMPORTANT: always leave a copy of 'world' in the stack

// global variables
static lua_State *L = NULL;
static MessageResponse (*message_callback)(Message*);
bool if_in_error = true;

// preprocessor
#define LUA_FIELD(c_field, field, type) { 	\
	lua_pushstring(L, #field);	\
	lua_gettable(L, -2);		\
	(c_field) = lua_to ## type(L, -1); \
	lua_pop(L, 1); }

#define LUA_PUSH_WORLD() { lua_getglobal(L, "world"); }
#define LUA_PUSH_HERO() { LUA_PUSH_WORLD(); lua_pushstring(L, "hero"); lua_gettable(L, -2); lua_remove(L, -2); }
#define LUA_PUSH_FUNCTION(f) { lua_pushstring(L, f); lua_gettable(L, -2); }
#define LUA_PUSH_METHOD(f) { LUA_PUSH_FUNCTION(f); lua_pushvalue(L, -2); }
#define LUA_CALL(narg, nres) { if(lua_pcall(L, (narg), (nres), 0) != LUA_OK) { if_error("%s\n", lua_tostring(L, -1)); } }

// function prototypes
extern int setenv (const char *, const char *, int);
static void check_stack();
static void if_error(const char *fmt, ...);
static void stack_dump();
static void if_person_on_stack(Person* person);

/*
 * INITIALIZATION
 */


void if_init()
{
	if_in_error = false;
	setenv("LUA_PATH_5_2", "../../engine/?.lua;;", 0);

	if(!L) {
		// first run
		L = luaL_newstate();
		luaL_openlibs(L);
	} else {
		// engine was reloaded, clear stack
		lua_settop(L, 0);
	}

	if(luaL_loadfile(L, "../../engine/world.lua") || lua_pcall(L, 0, 0, 0)) {
		if_error("can't load file: %s\n", lua_tostring(L, -1));
	}

	(void) stack_dump;
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
	if(if_in_error)
		return;

	check_stack();

	LUA_PUSH_WORLD();
	LUA_PUSH_METHOD("step");
	LUA_CALL(1, 0);
	lua_pop(L, 1);

	check_stack();
}


void if_hero_move(int speed, double direction)
{
	if(if_in_error)
		return;

	check_stack();

	LUA_PUSH_HERO();

	// set speed
	if(speed != 0) {
		LUA_PUSH_METHOD("turn_to");
		lua_pushnumber(L, direction);
		LUA_CALL(2, 0);
	}

	// set direction
	LUA_PUSH_METHOD("change_speed");
	lua_pushinteger(L, speed);
	LUA_CALL(2, 0);

	lua_pop(L, 1);

	check_stack();
}


/*
 * QUERIES
 */


void if_hero_position(double* x, double* y)
{
	if(if_in_error) {
		*x = *y = 0;
		return;
	}

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
	if(if_in_error)
		return 0;

	check_stack();

	// call function
	LUA_PUSH_WORLD();
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
	lua_pop(L, 2);

	check_stack();
	return n;
}


int if_people_visible(int x1, int y1, int x2, int y2, Person** people)
{
	if(if_in_error)
		return 0;

	check_stack();

	// call function
	LUA_PUSH_WORLD();
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
	lua_pop(L, 2);

	check_stack();
	return n;
}


/*
 * MESSAGE MANAGEMENT
 */
static int if_lua_message_callback(lua_State *L) {
	// check parameters
	Message msg;
	msg.text = luaL_checkstring(L, 1);
	msg.type = luaL_checkint(L, 2);
	if(lua_type(L, 3) != LUA_TNIL)
		luaL_checktype(L, 3, LUA_TTABLE);
	if(lua_type(L, 4) != LUA_TNIL)
		msg.image = luaL_checkint(L, 4);
	if(lua_type(L, 5) != LUA_TNIL)
		msg.person_id = luaL_checkint(L, 5);
	lua_pop(L, 5);

	// find options
	msg.options[0] = NULL;

	// call callback
	MessageResponse r = message_callback(&msg);
	if(msg.type == MESSAGE)
		lua_pushinteger(L, r.option);
	else
		lua_pushstring(L, r.input);
	return 1;
}

void if_register_dialog_callback(MessageResponse (*callback)(Message*))
{
	message_callback = callback;

	stack_dump();
	check_stack();

	lua_getglobal(L, "msg");
	lua_pushstring(L, "callback");
	lua_pushcfunction(L, if_lua_message_callback);
	lua_settable(L, -3);
	lua_pop(L, 1);

	check_stack();
}

int if_wrap(char* str, int columns, char*** ret)
{
	if(if_in_error)
		return 0;

	check_stack();

	// call function
	lua_getglobal(L, "string");
	LUA_PUSH_FUNCTION("wrap");
	lua_pushstring(L, str);
	lua_pushinteger(L, columns);
	LUA_CALL(2, 1);

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
	LUA_FIELD(person->direction, direction, number);
}


static void check_stack()
{
	if(lua_gettop(L) != 0) {
		stack_dump();
		abort();
	}
}

static void if_error(const char *fmt, ...) 
{
	va_list argp;
	va_start(argp, fmt);
	vfprintf(stderr, fmt, argp);
	va_end(argp);
	if_in_error = true;
	//lua_close(L);
	//exit(EXIT_FAILURE)
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

