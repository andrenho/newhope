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

typedef struct Car {
	double x, y;
	int model;
	double direction;
	double w, h;
} Car;

extern bool if_reload_engine;

//
// initialization
//
void if_init();
void if_init_world(int w, int h);
void if_finish();

//
// requests
//
void if_next_frame();
void if_player_move(int speed, double direction);
bool if_call(int narg, int nres);


//
// queries
//
void if_player_position(double* x, double* y);
int if_people_visible(int x1, int y1, int x2, int y2, Person** people);
int if_cars_visible(int x1, int y1, int x2, int y2, Car** car);
uint8_t if_world_tiles(int x, int y, BLOCK stack[10]);

//
// lua helper methods
//
int if_wrap(char* str, int columns, char*** ret);
void if_error(const char *fmt, ...);
void check_stack();

#define LUA_FIELD(c_field, field, type) { 	\
	lua_pushstring(L, field);	\
	lua_gettable(L, -2);		\
	(c_field) = lua_to ## type(L, -1); \
	lua_pop(L, 1); }

#define LUA_PUSH_WORLD() { lua_getglobal(L, "world"); }
#define LUA_PUSH_PLAYER() { LUA_PUSH_WORLD(); lua_pushstring(L, "player"); lua_gettable(L, -2); lua_remove(L, -2); }
#define LUA_PUSH_FUNCTION(f) { lua_pushstring(L, f); lua_gettable(L, -2); }
#define LUA_PUSH_METHOD(f) { LUA_PUSH_FUNCTION(f); lua_pushvalue(L, -2); }

#endif
