#include "ui.h"

#include <lauxlib.h>
#include <SDL2/SDL.h>

#include "luah.h"
#include "wireframe.h"

typedef struct UI {
	SDL_Window* win;
	SDL_Renderer* ren;
	bool wireframe_mode;
} UI;
static UI ui;


int ui_c_init(lua_State* L)
{
	ui.wireframe_mode = true;

	// initialize SDL
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "\nUnable to initialize SDL: %s\n", SDL_GetError());
	}

	// initialize window and renderer
	if(SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_RESIZABLE, &ui.win, 
				&ui.ren) != 0) {
		fprintf(stderr, "\nUnable to intialize window: %s\n", SDL_GetError());
	}

	lua_pushlightuserdata(L, &ui);
	return 1;
}


int ui_now(lua_State* L)
{
	lua_pushinteger(L, SDL_GetTicks());
	return 1;
}


int ui_wait(lua_State* L)
{
	SDL_Delay(luaL_checkinteger(L, 2));
	return 0;
}


int ui_render(lua_State* L)
{
	SDL_SetRenderDrawColor(ui.ren, 255, 255, 255, 255);
	SDL_RenderClear(ui.ren);

	SDL_SetRenderDrawColor(ui.ren, 0, 0, 0, 255);

	if(ui.wireframe_mode) {
		wireframe_render(L, ui.ren);
	} else {
		abort();
	}

	SDL_RenderPresent(ui.ren);
	return 0;
}


int ui_user_events(lua_State* L)
{
	lua_newtable(L);

	SDL_Event e;
	int i = 1;
	while(SDL_PollEvent(&e)) {
		switch(e.type) {
		case SDL_QUIT:
			lua_newtable(L);
			luah_stack_dump(L);
			LUA_SET_FIELD(L, "quit", "event", string);
			lua_rawseti(L, -2, i++);
			break;
		}
	}

	return 1;
}


int ui_visible_tiles(lua_State* L)
{
	lua_pushnumber(L, -50);
	lua_pushnumber(L, -50);
	lua_pushnumber(L, 50);
	lua_pushnumber(L, 50);
	return 4;
}


int ui_clean_up(lua_State* L)
{
	SDL_DestroyRenderer(ui.ren);
	SDL_DestroyWindow(ui.win);
	SDL_Quit();
	return 0;
}
