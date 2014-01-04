#ifndef MINIMAP_H
#define MINIMAP_H

#include <SDL2/SDL.h>
#include <lua.h>

typedef struct Minimap {
	int w, h;
	SDL_Texture* tx;
} Minimap;

Minimap* minimap_new(lua_State* L, SDL_Renderer* ren, int w, int h);
void minimap_destroy(Minimap** mm);

#endif