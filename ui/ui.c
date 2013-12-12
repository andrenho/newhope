#include "ui.h"

#include "SDL2/SDL.h"

#include "physics.h"

SDL_Window* win;
SDL_Renderer* ren;

int cb_initialize_ui(lua_State* L)
{
	// initialize SDL
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "\nUnable to initialize SDL: %s\n", SDL_GetError());
	}

	// initialize window and renderer
	if(SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_RESIZABLE, &win, 
				&ren) != 0) {
		fprintf(stderr, "\nUnable to intialize window: %s\n", SDL_GetError());
	}

	// initialize physics
	physics_init();

	return 0;
}


int cb_active(lua_State* L)
{
	lua_pushboolean(L, 0);
	return 1;
}


int cb_finish_ui(lua_State* L)
{
	physics_finish();
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}


int cb_current_time_us(lua_State* L)
{
	lua_pushnumber(L, 0);
	return 1;
}


int cb_get_commands(lua_State* L)
{
	lua_newtable(L);
	return 1;
}


int cb_sleep_us(lua_State* L)
{
	return 0;
}
