#ifndef WIREFRAME_H
#define WIREFRAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <lua.h>

void wireframe_render(lua_State* L, SDL_Window* win, SDL_Renderer* ren);
int wireframe_message(lua_State* L, SDL_Window* win, SDL_Renderer* ren, 
		TTF_Font* font);
int wireframe_visible_tiles(lua_State* L, SDL_Window* win);

#endif
