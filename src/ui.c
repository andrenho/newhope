#include "ui.h"

#include <stdlib.h>
#include <string.h>

#include <lauxlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "luah.h"
#include "minimap.h"
#include "wireframe.h"

typedef struct UI {
	SDL_Window* win;
	SDL_Renderer* ren;
	bool wireframe_mode;
	TTF_Font* main_font;
	Minimap* mm;
} UI;
static UI ui;

const double Z = 10;
int rx = 50, ry = 50;

int ui_c_init(lua_State* L)
{
	ui.wireframe_mode = true;

	// initialize SDL
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "\nUnable to initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	// initialize window
	if(!(ui.win = SDL_CreateWindow("NewHope", SDL_WINDOWPOS_CENTERED, 
			SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_RESIZABLE))) {
		fprintf(stderr, "\nUnable to intialize window: %s\n", SDL_GetError());
		exit(1);
	}

	// initialize renderer
	if(!(ui.ren = SDL_CreateRenderer(ui.win, -1, SDL_RENDERER_SOFTWARE))) {
		fprintf(stderr, "\nUnable to intialize renderer: %s\n", SDL_GetError());
		exit(1);
	}

	// initialize TTF_Init
	if(TTF_Init() == -1) {
		fprintf(stderr, "\nError initializing SDL2_ttf: %s\n", TTF_GetError());
		exit(1);
	}

	// load font
	ui.main_font = TTF_OpenFont(DATADIR "/PressStart2P.ttf", 16);
	if(!ui.main_font) {
		fprintf(stderr, "\nUnable to load font: %s\n", TTF_GetError());
		exit(1);
	}

	// create minimap
	ui.mm = minimap_new(L, ui.ren, 250, 250);

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
		wireframe_render(L, ui.win, ui.ren);
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
			LUA_SET_FIELD(L, "quit", "event", string);
			lua_rawseti(L, -2, i++);
			break;
		case SDL_KEYDOWN:
			lua_newtable(L);
			LUA_SET_FIELD(L, "key", "event", string);
			LUA_SET_FIELD(L, SDL_GetKeyName(e.key.keysym.sym), "key", string);
			lua_rawseti(L, -2, i++);
			break;
		}
	}

	return 1;
}


int ui_keyboard_state(lua_State* L)
{
	const Uint8* k = SDL_GetKeyboardState(NULL);

	lua_createtable(L, 0, 5);
	LUA_SET_FIELD(L, k[SDL_SCANCODE_UP], "up", boolean);
	LUA_SET_FIELD(L, k[SDL_SCANCODE_DOWN], "down", boolean);
	LUA_SET_FIELD(L, k[SDL_SCANCODE_LEFT], "left", boolean);
	LUA_SET_FIELD(L, k[SDL_SCANCODE_RIGHT], "right", boolean);
	return 1;
}


int ui_center_screen(lua_State* L)
{
	int win_w, win_h;
	SDL_GetWindowSize(ui.win, &win_w, &win_h);

	// center screen on player
	double x = luaL_checknumber(L, 2);
	double y = luaL_checknumber(L, 3);
	rx = -x*Z + (win_w/2);
	ry = -y*Z + (win_h/2);

	return 0;
}


int ui_visible_tiles(lua_State* L)
{
	if(ui.wireframe_mode) {
		return wireframe_visible_tiles(L, ui.win);
	} else {
		abort();
	}
}


int ui_show_minimap(lua_State* L)
{
	int win_w, win_h;
	SDL_GetWindowSize(ui.win, &win_w, &win_h);

	SDL_SetRenderDrawColor(ui.ren, 0x96, 0x4b, 0x00, SDL_ALPHA_OPAQUE);
	SDL_Rect r = { win_w/2 - ui.mm->w/2 - 5, win_h/2 - ui.mm->h/2 - 5, 
		ui.mm->w + 10, ui.mm->h + 10 };
	SDL_RenderFillRect(ui.ren, &r);

	SDL_Rect r2 = { r.x+5, r.y+5, r.w-10, r.h-10 };
	SDL_RenderCopy(ui.ren, ui.mm->tx, NULL, &r2);

	SDL_RenderPresent(ui.ren);

	SDL_Event e;
	for(;;) {
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_KEYDOWN) {
				return 0;
			}
		}
	}
}


int ui_message(lua_State* L)
{
	if(ui.wireframe_mode) {
		return wireframe_message(L, ui.win, ui.ren, ui.main_font);
	} else {
		abort();
	}
}


int ui_clean_up(lua_State* L)
{
	SDL_DestroyRenderer(ui.ren);
	SDL_DestroyWindow(ui.win);
	SDL_Quit();
	return 0;
}


int ui_wrap_text(char* text, int chars)
{
	int i, len = strlen(text);
	int nl = 0, lines = 1;
	for(i=0; i<len; i++) {
		if(nl > chars) {
			lines++;
			nl = 0;
			while(text[i--] != ' ') { }
			text[i+1] = '\n';
		}
		nl++;
	}

	return lines;
}
