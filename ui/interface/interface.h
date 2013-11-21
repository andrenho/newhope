#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdbool.h>
#include <stdint.h>

#include "callbacks.h"

typedef uint16_t BLOCK;

typedef struct Person {
	double x, y;
	double direction;
	int image;
} Person;

extern bool if_in_error;

//
// initialization
//
void if_init();
void if_init_world(int w, int h);
void if_install_callbacks(
		MessageResponse (*callback)(Message*));
void if_finish();

//
// requests
//
void if_next_frame();
void if_hero_move(int speed, double direction);
void check_stack();

//
// queries
//
void if_hero_position(double* x, double* y);
int if_people_visible(int x1, int y1, int x2, int y2, Person** people);
uint8_t if_world_tiles(int x, int y, BLOCK stack[10]);

//
// lua helper methods
//
int if_wrap(char* str, int columns, char*** ret);
void if_error(const char *fmt, ...);

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

#endif
