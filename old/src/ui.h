#ifndef UI_H
#define UI_H

#include <stdbool.h>

#include <lua.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "minimap.h"

typedef struct UI {
	SDL_Window* win;
	SDL_Renderer* ren;
	TTF_Font* main_font;
	Minimap* mm;
} UI;
extern UI ui;

int ui_c_init(lua_State* L);
int ui_now(lua_State* L);
int ui_wait(lua_State* L);
int ui_render(lua_State* L);
int ui_user_events(lua_State* L);
int ui_keyboard_state(lua_State* L);
int ui_visible_tiles(lua_State* L);
int ui_center_screen(lua_State* L);
int ui_show_minimap(lua_State* L);
int ui_clean_up(lua_State* L);

int ui_wrap_text(char* text, int chars);

#endif
