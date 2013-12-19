#include "ui.h"

#include "SDL2/SDL.h"
#include "lauxlib.h"

#include "interface.h"
#include "physics.h"
#include "render.h"

SDL_Window* win;
SDL_Renderer* ren;

static int active = 1;

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
	SDL_SetWindowTitle(win, "New Hope " VERSION);

	init_render();

	// initialize physics
	physics_init();

	return 0;
}


int cb_active(lua_State* L)
{
	lua_pushboolean(L, active);
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


int cb_current_time_ms(lua_State* L)
{
	lua_pushnumber(L, SDL_GetTicks());
	return 1;
}


int cb_get_commands(lua_State* L)
{
	lua_newtable(L);

	// get events
	SDL_Event e;
	while(SDL_PollEvent(&e)) {
		switch(e.type)
		{
		case SDL_QUIT:
			active = 0;
			break;
		}
	}

	// get directional situation
	const uint8_t* s = SDL_GetKeyboardState(NULL);
	LUA_SET_FIELD(s[SDL_SCANCODE_DOWN],  "down",  boolean);
	LUA_SET_FIELD(s[SDL_SCANCODE_UP],    "up",    boolean);
	LUA_SET_FIELD(s[SDL_SCANCODE_LEFT],  "left",  boolean);
	LUA_SET_FIELD(s[SDL_SCANCODE_RIGHT], "right", boolean);

	return 1;
}


int cb_sleep_ms(lua_State* L)
{
	SDL_Delay(luaL_checkinteger(L, 1));
	return 0;
}
